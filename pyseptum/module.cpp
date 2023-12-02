#include <eigenpy/eigenpy.hpp>

#include "rerun_visualizer.hpp"

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

namespace bp = boost::python;

PYMODULE() {
  using namespace pinviz;
  using pinocchio::GeometryModel;
  using pinocchio::Model;

  bp::import("pinocchio");

  bp::class_<RerunVisualizer, boost::noncopyable>("RerunVisualizer",
                                                  bp::no_init)
      .def(bp::init<Model const &, GeometryModel const &>(
          bp::args("self", "model", "geomModel")))
      .def("initViewer", &RerunVisualizer::initViewer, bp::args("self"))
      .def("display", &RerunVisualizer::display, bp::args("self", "q"))
      .def("updatePlacements", &RerunVisualizer::updatePlacements,
           bp::args("self"))
      .def_readonly("data", &RerunVisualizer::data)
      .def_readonly("visualData", &RerunVisualizer::visualData)
      .add_property("isInitialized", &RerunVisualizer::isInitialized);
}
