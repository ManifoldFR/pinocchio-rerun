#include "pinviz.hpp"

namespace pinviz {

RerunVisualizer::RerunVisualizer(const pinocchio::GeometryModel &geomModel)
    : stream("RerunVisualizer"), visualModel(geomModel),
      visualData(visualModel) {
  auto e = stream.spawn();
  e.exit_on_failure();
}

void RerunVisualizer::initViewer() {
  loadPinocchioModel(visualModel, stream, "pinviz/");
}

} // namespace pinviz
