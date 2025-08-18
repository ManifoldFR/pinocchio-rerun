#include <eigenpy/eigenpy.hpp>
#include <eigenpy/optional.hpp>
#include <eigenpy/std-vector.hpp>
#include <pinocchio/bindings/python/visualizers/visualizer-visitor.hpp>

#include "pinocchio_rerun.hpp"

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
      .def("__init__",
           bp::make_constructor(
               +[](const pinocchio::Model &model,
                   const pinocchio::GeometryModel &geomModel,
                   const std::string &appID, const std::string &recID) {
                 return new RerunVisualizer(model, geomModel, appID, recID);
               },
               bp::default_call_policies(),
               (bp::arg("model"), bp::arg("geom_model"),
                bp::arg("app_id") = "RerunVisualizer",
                bp::arg("rec_id") = "DefaultID")))
      .def(pinocchio::python::VisualizerPythonVisitor<RerunVisualizer>())
      .add_property("initialized", &RerunVisualizer::initialized)
      .add_property("recordingID", &RerunVisualizer::recordingID)
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
