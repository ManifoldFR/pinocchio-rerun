#pragma once

#include "pinocchio.hpp"
#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

class RerunVisualizer {
public:
  RerunVisualizer(const pinocchio::GeometryModel &geomModel);

  rerun::RecordingStream stream;
  const pinocchio::GeometryModel &visualModel;
  pinocchio::GeometryData visualData;
};

} // namespace pinviz
