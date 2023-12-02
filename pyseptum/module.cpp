#include <eigenpy/eigenpy.hpp>

#include "pinviz.hpp"

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

namespace bp = boost::python;

PYMODULE() {
  using namespace pinviz;
  using pinocchio::GeometryModel;

  bp::class_<RerunVisualizer, boost::noncopyable>("RerunVisualizer",
                                                  bp::no_init)
      .def(bp::init<GeometryModel const &>(bp::args("self", "geomModel")))
      .def("initViewer", &RerunVisualizer::initViewer, bp::args("self"));
}
