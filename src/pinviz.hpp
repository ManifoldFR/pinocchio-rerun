#pragma once

#include "pinocchio.hpp"

namespace pinviz {

class RerunVisualizer {
public:
  RerunVisualizer() : stream("RerunVisualizer") {
    stream.spawn().exit_on_failure();
  }

  void loadModel(const pinocchio::GeometryModel &geomModel);

  rerun::RecordingStream stream;
  typename pinocchio::Model::Data data;
};

} // namespace pinviz
