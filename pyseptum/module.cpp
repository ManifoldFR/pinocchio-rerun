#include <eigenpy/eigenpy.hpp>

#include "RerunVisualizer.hpp"

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

namespace bp = boost::python;

PYMODULE() {
  using namespace pinviz;
  using pinocchio::GeometryModel;
  using pinocchio::Model;

  bp::class_<RerunVisualizer, boost::noncopyable>("RerunVisualizer",
                                                  bp::no_init)
      .def(bp::init<Model const &, GeometryModel const &>(
          bp::args("self", "model", "geomModel")))
      .def("initViewer", &RerunVisualizer::initViewer, bp::args("self"))
      .def("display", &RerunVisualizer::display, bp::args("self", "q"));
}
