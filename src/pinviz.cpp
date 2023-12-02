#include "pinviz.hpp"

namespace pinviz {

RerunVisualizer::RerunVisualizer(const pinocchio::GeometryModel &geomModel)
    : stream("RerunVisualizer"), visualModel(geomModel), visualData(geomModel) {
  stream.spawn().exit_on_failure();
  loadPinocchioModel(geomModel, stream);
}

} // namespace pinviz
