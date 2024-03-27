// modified_tagged_file_sink_python.cc
#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/modified_tagged_file_sink_module/modified_tagged_file_sink.h>

#include <modified_tagged_file_sink_pydoc.h>

void bind_modified_tagged_file_sink(py::module& m)
{
    using modified_tagged_file_sink    = gr::modified_tagged_file_sink_module::modified_tagged_file_sink;
    py::class_<modified_tagged_file_sink, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<modified_tagged_file_sink>>(m, "modified_tagged_file_sink", D(modified_tagged_file_sink))
        .def(py::init(&modified_tagged_file_sink::make),
             py::arg("filename"),
             py::arg("append"),
             py::arg("itemsize"),
             py::arg("samp_rate"),
             D(modified_tagged_file_sink,make))
        .def("set_filename",
             &modified_tagged_file_sink::set_filename,
             py::arg("filename"),
             D(modified_tagged_file_sink, set_filename))
        .def("filename", &modified_tagged_file_sink::filename, D(modified_tagged_file_sink, filename))
        ;
}





