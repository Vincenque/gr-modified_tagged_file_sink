// modified_tagged_file_sink_impl.h
#ifndef INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_IMPL_H
#define INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_IMPL_H

#include <string>
#include <iostream>
using namespace std;

#include <gnuradio/blocks/api.h>
#include <gnuradio/modified_tagged_file_sink_module/modified_tagged_file_sink.h>
#include <cstdio>



namespace gr {
namespace modified_tagged_file_sink_module {

class modified_tagged_file_sink_impl : public modified_tagged_file_sink
{
private:
    enum class state_t { NOT_IN_BURST = 0, IN_BURST };
    string d_filename;
    string d_previous_filename;
    bool d_append;
    const size_t d_itemsize;
    const double d_sample_rate;
    state_t d_state;
    FILE* d_handle;
    int d_n;
    uint64_t d_last_N;
    double d_timeval;

public:
    modified_tagged_file_sink_impl(string filename, bool append, size_t itemsize, double samp_rate);
    ~modified_tagged_file_sink_impl();

    void set_filename(string filename) override
    {
        d_filename = filename;
    }
    string filename() const override { return d_filename; }

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace modified_tagged_file_sink_module
} // namespace gr

#endif /* INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_IMPL_H */
