/* -*- c++ -*- */
/*
 * Copyright 2024 Witold Duda.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H
#define INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H

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
     * To avoid accidental use of raw pointers,
     * modified_tagged_file_sink_module::modified_tagged_file_sink's constructor is in a
     * private implementation class.
     * modified_tagged_file_sink_module::modified_tagged_file_sink::make is the public
     * interface for creating new instances.
     */
    static sptr make(size_t itemsize, double samp_rate);
};

} // namespace modified_tagged_file_sink_module
} // namespace gr

#endif /* INCLUDED_MODIFIED_TAGGED_FILE_SINK_MODULE_MODIFIED_TAGGED_FILE_SINK_H */