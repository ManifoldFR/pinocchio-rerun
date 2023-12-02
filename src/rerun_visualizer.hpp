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

  void display(const std::optional<ConstVectorRef> &q = std::nullopt);

  void updatePlacements();

  inline const std::string &prefix() const { return m_prefix; }
  inline void switchTimeline(const std::string &name) {
    stream.set_time_sequence(name, 0L);
  }
  inline void disableTimeline(const std::string &name) {
    stream.disable_timeline(name);
  }

  rerun::RecordingStream stream;
  const pinocchio::Model &model;
  pinocchio::Data data;
  const pinocchio::GeometryModel &visualModel;
  pinocchio::GeometryData visualData;

  inline bool initialized() const { return m_initialized; }

protected:
  std::string m_prefix;
  bool m_initialized;
};

} // namespace pinviz
