#pragma once

#include "pinocchio.hpp"
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

class RerunVisualizer {
public:
  RerunVisualizer(const pinocchio::Model &model,
                  const pinocchio::GeometryModel &geomModel);

  void initViewer();

  void display(ConstVectorRef const &q) {
    pinocchio::forwardKinematics(model, data, q);

    int nframes = model.nframes;
  }

  rerun::RecordingStream stream;
  const pinocchio::Model &model;
  pinocchio::Data data;
  const pinocchio::GeometryModel &visualModel;
  pinocchio::GeometryData visualData;
  std::string prefix;
};

} // namespace pinviz
