#pragma once

#include "pinocchio.hpp"

#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

template <typename Scalar>
auto pinSE3toRerun(const pinocchio::SE3Tpl<Scalar, Eigen::ColMajor> &tr_) {
  auto tr = tr_.template cast<float>();
  return rerun::Transform3D{rerun::Vec3D(tr.translation().data()),
                            rerun::Mat3x3(tr.rotation().data())};
}

class RerunVisualizer {
public:
  RerunVisualizer(const pinocchio::Model &model,
                  const pinocchio::GeometryModel &geomModel);

  void initViewer();

  void display(const ConstVectorRef &q);

  void updatePlacements();

  rerun::RecordingStream stream;
  const pinocchio::Model &model;
  pinocchio::Data data;
  const pinocchio::GeometryModel &visualModel;
  pinocchio::GeometryData visualData;
  std::string prefix;

protected:
  bool m_isInitialized;
};

} // namespace pinviz
