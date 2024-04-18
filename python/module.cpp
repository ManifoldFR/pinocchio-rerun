#include <eigenpy/eigenpy.hpp>
#include <eigenpy/optional.hpp>
#include <eigenpy/std-vector.hpp>
#include <pinocchio_visualizers/python/visitor.hpp>

#include "septum.hpp"

#define PYMODULE() BOOST_PYTHON_MODULE(MODULE_NAME)

namespace bp = boost::python;

bp::arg operator""_a(const char *argname, std::size_t) {
  return bp::arg(argname);
}

template <typename M>
auto convertToStdVecConstRef(const std::vector<Eigen::Ref<M>> &xs_) {
  std::vector<Eigen::Ref<const M>> xs;
  for (size_t i = 0; i < xs_.size(); i++) {
    xs.emplace_back(xs_[i]);
  }
  return xs;
}

PYMODULE() {
  using namespace pinrerun;
  using pinocchio::GeometryModel;
  using pinocchio::Model;

  bp::scope().attr("__version__") = printVersion();

  eigenpy::enableEigenPy();
  bp::import("pinocchio");

  eigenpy::OptionalConverter<ConstVectorRef, std::optional>::registration();
  eigenpy::StdVectorPythonVisitor<vector<VectorRef>, false>::expose(
      "StdVec_VectorRef");

  bp::class_<RerunVisualizer, boost::noncopyable>("RerunVisualizer",
                                                  bp::no_init)
      .def(bp::init<Model const &, GeometryModel const &>(
          ("self"_a, "model", "geomModel")))
      .def(pinviz::VisualizerVisitor<RerunVisualizer>())
      .add_property("initialized", &RerunVisualizer::initialized)
      .def("switchTimeline", &RerunVisualizer::switchTimeline,
           ("self"_a, "name"_a), "Switch Rerun timelines.")
      .def("disableTimeline", &RerunVisualizer::disableTimeline,
           ("self"_a, "name"_a), "Disable a Rerun timeline.")
      .def("drawFrameVelocities", &RerunVisualizer::drawFrameVelocities,
           ("self"_a, "frame_ids"_a))
      .def(
          "play",
          +[](RerunVisualizer &v, const vector<VectorRef> &qs, double dt,
              std::string timeline) {
            v.play(convertToStdVecConstRef(qs), dt, timeline);
          },
          ("self"_a, "qs"_a, "dt"_a, "timeline"_a = "trajectory"));
}
