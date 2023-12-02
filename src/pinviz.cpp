#include "pinviz.hpp"

#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

void RerunVisualizer::loadModel(const pinocchio::GeometryModel &geomModel) {
  loadPinocchioModel(geomModel, stream);
}

} // namespace pinviz
