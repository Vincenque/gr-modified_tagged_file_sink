// modified_tagged_file_sink_impl.cc
#include "modified_tagged_file_sink_impl.h"
#include <gnuradio/io_signature.h>

#include <fcntl.h>
#include <cerrno>
#include <cstdint>

#ifdef HAVE_IO_H
#include <io.h>
#endif

#ifdef O_BINARY
#define OUR_O_BINARY O_BINARY
#else
#define OUR_O_BINARY 0
#endif

// should be handled via configure
#ifdef O_LARGEFILE
#define OUR_O_LARGEFILE O_LARGEFILE
#else
#define OUR_O_LARGEFILE 0
#endif

namespace gr {
namespace modified_tagged_file_sink_module {

modified_tagged_file_sink::sptr modified_tagged_file_sink::make(string filename,
                                                                bool append,
                                                                size_t itemsize,
                                                                double samp_rate)
{
    return gnuradio::make_block_sptr<modified_tagged_file_sink_impl>(
        filename, append, itemsize, samp_rate);
}

modified_tagged_file_sink_impl::modified_tagged_file_sink_impl(string filename,
                                                               bool append,
                                                               size_t itemsize,
                                                               double samp_rate)
    : gr::sync_block("modified_tagged_file_sink",
                     io_signature::make(1, 1, itemsize),
                     io_signature::make(0, 0, 0)),
      d_filename(filename),
      d_previous_filename(filename),
      d_append(append),
      d_itemsize(itemsize),
      d_sample_rate(samp_rate),
      d_state(state_t::NOT_IN_BURST),
      d_handle(nullptr),
      d_n(0),
      d_last_N(0),
      d_timeval(0)
{
}

modified_tagged_file_sink_impl::~modified_tagged_file_sink_impl() {}

int modified_tagged_file_sink_impl::work(int noutput_items,
                                         gr_vector_const_void_star& input_items,
                                         gr_vector_void_star& output_items)
{
    char* inbuf = (char*)input_items[0];

    uint64_t start_N = nitems_read(0);
    uint64_t end_N = start_N + (uint64_t)(noutput_items);
    pmt::pmt_t bkey = pmt::string_to_symbol("burst");
    pmt::pmt_t tkey = pmt::string_to_symbol("rx_time"); // use gr_tags::key_time

    std::vector<tag_t> all_tags;
    get_tags_in_range(all_tags, 0, start_N, end_N);
    std::vector<tag_t>::iterator vitr = all_tags.begin();

    // Look for a time tag and initialize d_timeval.
    std::vector<tag_t> time_tags_outer;
    get_tags_in_range(time_tags_outer, 0, start_N, end_N, tkey);
    if (!time_tags_outer.empty()) {
        const tag_t tag = time_tags_outer[0];
        uint64_t offset = tag.offset;
        pmt::pmt_t time = tag.value;
        uint64_t tsecs = pmt::to_uint64(pmt::tuple_ref(time, 0));
        double tfrac = pmt::to_double(pmt::tuple_ref(time, 1));
        double delta = (double)offset / d_sample_rate;
        d_timeval = (double)tsecs + tfrac + delta;
        d_last_N = offset;
    }

    uint64_t idx = 0;
    uint64_t idx_stop = 0;
    while (idx < (unsigned int)noutput_items) {
        if (d_state == state_t::NOT_IN_BURST) {
            while (vitr != all_tags.end()) {
                if ((pmt::eqv((*vitr).key, bkey)) && pmt::is_true((*vitr).value)) {

                    auto N = (*vitr).offset;
                    idx = (N - start_N);

                    // std::cout << std::endl << "Found start of burst: "
                    //	    << idx << ", " << N << std::endl;

                    // Find time burst occurred by getting latest time tag and
                    // extrapolating to new time based on sample rate of this block.
                    std::vector<tag_t> time_tags;
                    // get_tags_in_range(time_tags, 0, d_last_N, N, gr_tags::key_time);
                    get_tags_in_range(time_tags, 0, d_last_N, N, tkey);
                    if (!time_tags.empty()) {
                        const tag_t tag = time_tags[time_tags.size() - 1];

                        uint64_t time_nitems = tag.offset;

                        // Get time based on last time tag from USRP
                        pmt::pmt_t time = tag.value;
                        uint64_t tsecs = pmt::to_uint64(pmt::tuple_ref(time, 0));
                        double tfrac = pmt::to_double(pmt::tuple_ref(time, 1));

                        // Get new time from last time tag + difference in time to when
                        // burst tag occurred based on the sample rate
                        double delta = (double)(N - time_nitems) / d_sample_rate;
                        d_timeval = (double)tsecs + tfrac + delta;

                        // std::cout.setf(std::ios::fixed, std::ios::floatfield);
                        // std::cout.precision(8);
                        // std::cout << "Time found: " << (double)tsecs + tfrac <<
                        // std::endl; std::cout << "   time: " << d_timeval << std::endl;
                        // std::cout << "   time at N = " << time_nitems << " burst N = "
                        // << N << std::endl;
                    } else {
                        // if no time tag, use last seen tag and update time based on
                        // sample rate of the block
                        d_timeval += (double)(N - d_last_N) / d_sample_rate;
                        // std::cout << "Time not found" << std::endl;
                        // std::cout << "   time: " << d_timeval << std::endl;
                    }
                    d_last_N = N;
                    if (d_filename != d_previous_filename) {
                        d_previous_filename = d_filename;
                        d_n = 0;
                    }
                    std::string file_name = fmt::format("{:s}_{:d}.bin", d_filename, d_n);
                    d_logger->trace("New file_name '{:s}'", file_name);
                    d_n++;

                    int fd;
                    if ((fd = ::open(file_name.c_str(),
                                     O_WRONLY | O_CREAT | O_TRUNC | OUR_O_LARGEFILE |
                                         OUR_O_BINARY,
                                     0664)) < 0) {
                        d_logger->error("::open {:s}:{:s}", file_name, strerror(errno));
                        return -1;
                    }

                    // FIXME:
                    // if((d_handle = fdopen (fd, d_is_binary ? "wb" : "w")) == NULL) {
                    if ((d_handle = fdopen(fd, "wb")) == NULL) {
                        d_logger->error("fdopen {:s}:{:s}", file_name, strerror(errno));
                        ::close(fd); // don't leak file descriptor if fdopen fails.
                    }

                    // std::cout << "Created new file: " << file_name.str() << std::endl;

                    d_state = state_t::IN_BURST;
                    break;
                }

                vitr++;
            }
            if (d_state == state_t::NOT_IN_BURST)
                return noutput_items;
        } else { // In burst
            while (vitr != all_tags.end()) {
                if ((pmt::eqv((*vitr).key, bkey)) && pmt::is_false((*vitr).value)) {
                    auto N = (*vitr).offset;
                    idx_stop = N - start_N;

                    // std::cout << "Found end of burst: "
                    //	    << idx_stop << ", " << N << std::endl;

                    int count = fwrite(
                        &inbuf[d_itemsize * idx], d_itemsize, idx_stop - idx, d_handle);
                    if (count == 0) {
                        if (ferror(d_handle)) {
                            d_logger->error("writing file(1): {:s}", strerror(errno));
                        }
                    }
                    idx = idx_stop;
                    d_state = state_t::NOT_IN_BURST;
                    vitr++;
                    fclose(d_handle);
                    break;
                } else {
                    vitr++;
                }
            }
            if (d_state == state_t::IN_BURST) {
                int count = fwrite(
                    &inbuf[d_itemsize * idx], d_itemsize, noutput_items - idx, d_handle);
                if (count == 0) {
                    if (ferror(d_handle)) {
                        d_logger->error("writing file(2): {:s}", strerror(errno));
                    }
                }
                idx = noutput_items;
            }
        }
    }

    return noutput_items;
}

} /* namespace modified_tagged_file_sink_module */
} /* namespace gr */
