#include "RerunVisualizer.hpp"

namespace pinviz {

RerunVisualizer::RerunVisualizer(const pinocchio::Model &model,
                                 const pinocchio::GeometryModel &geomModel)
    : stream("RerunVisualizer"), model(model), data(model),
      visualModel(geomModel), visualData(visualModel) {
  auto e = stream.spawn();
  e.exit_on_failure();
  prefix = "pin/" + model.name + "/";
}

void RerunVisualizer::initViewer() {
  loadPinocchioModel(visualModel, stream, prefix);
}

} // namespace pinviz
