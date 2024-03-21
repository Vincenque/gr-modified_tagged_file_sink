/* -*- c++ -*- */
/*
 * Copyright 2024 Witold Duda.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

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
 * \brief <+description of block+>
 * \ingroup modified_tagged_file_sink_module
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
     * \param filename Name of file
     * \param append Append file or overwrite
     * \param itemsize Size of item
     * \param samp_rate Sample rate
     */
    static sptr make(string filename, bool append, size_t itemsize, double samp_rate);
    /*!
     * \brief Set the filename
     *
     * This method allows you to dynamically change the filename of the block.
     *
     * \param filename The new filename
     */
    virtual void set_filename(string filename) = 0;
    virtual string filename() const = 0;
};

} // namespace modified_tagged_file_sink_module
} // namespace gr

#endif /* INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H */
