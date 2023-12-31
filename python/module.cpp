#include <eigenpy/eigenpy.hpp>
#include <eigenpy/optional.hpp>
#include <eigenpy/std-vector.hpp>

#include "septum.hpp"

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

namespace bp = boost::python;

bp::arg operator""_a(const char *argname, std::size_t) {
  return bp::arg(argname);
}

PYMODULE() {
  using namespace pinviz;
  using pinocchio::GeometryModel;
  using pinocchio::Model;

  bp::scope().attr("__version__") = printVersion();

  eigenpy::enableEigenPy();
  bp::import("pinocchio");

  eigenpy::OptionalConverter<ConstVectorRef, std::optional>::registration();
  eigenpy::StdVectorPythonVisitor<vector<VectorRef>, false>::expose(
      "StdVec_ConstVectorRef");

  bp::class_<RerunVisualizer, boost::noncopyable>("RerunVisualizer",
                                                  bp::no_init)
      .def(bp::init<Model const &, GeometryModel const &>(
          ("self"_a, "model", "geomModel")))
      .def("initViewer", &RerunVisualizer::initViewer, bp::args("self"))
      .def("display", &RerunVisualizer::display,
           ("self"_a, "q"_a = std::nullopt))
      .def("updatePlacements", &RerunVisualizer::updatePlacements, "self"_a)
      .def_readonly("data", &RerunVisualizer::data)
      .def_readonly("visualData", &RerunVisualizer::visualData)
      .add_property("initialized", &RerunVisualizer::initialized)
      .def("switchTimeline", &RerunVisualizer::switchTimeline,
           ("self"_a, "name"_a), "Switch Rerun timelines.")
      .def("disableTimeline", &RerunVisualizer::disableTimeline,
           ("self"_a, "name"_a), "Disable a Rerun timeline.")
      .def("drawFrameVelocities", &RerunVisualizer::drawFrameVelocities,
           ("self"_a, "frame_ids"_a))
      .def("play", &RerunVisualizer::play,
           ("self"_a, "qs"_a, "dt"_a, "timeline"_a = "trajectory"));
}
