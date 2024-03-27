// modified_tagged_file_sink.h
#ifndef INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H
#define INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H

#include <iostream>
#include <string>
using namespace std;

#include <gnuradio/modified_tagged_file_sink_module/api.h>
#include <gnuradio/sync_block.h>


namespace gr {
namespace modified_tagged_file_sink_module {

/*!
 * \brief This class represents a block that writes data to a file in a GNU Radio
 * flowgraph. \ingroup modified_tagged_file_sink_module
 *
 */
class MODIFIED_TAGGED_FILE_SINK_MODULE_API modified_tagged_file_sink
    : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<modified_tagged_file_sink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * modified_tagged_file_sink_module::modified_tagged_file_sink.
     *
     * \param filename The name of the file to which data will be written.
     * \param append If true, data will be appended to the file. If false, the file will
     * be overwritten. 
     * \param itemsize The size of each data item in bytes. 
     * \param samp_rate The sample rate of the data.
     */
    static sptr make(string filename, bool append, size_t itemsize, double samp_rate);

    /*!
     * \brief Set the filename.
     *
     * This method allows you to dynamically change the filename of the block.
     *
     * \param filename The new filename.
     */
    virtual void set_filename(string filename) = 0;
    virtual string filename() const = 0;
};

} // namespace modified_tagged_file_sink_module
} // namespace gr

#endif /* INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H */
