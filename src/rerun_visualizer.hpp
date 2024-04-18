#pragma once

#include "pinocchio.hpp"

#include <pinocchio_visualizers/base-visualizer.hpp>

namespace pinrerun {

template <typename Scalar>
auto pinSE3toRerun(const pinocchio::SE3Tpl<Scalar, Eigen::ColMajor> &tr_) {
  auto tr = tr_.template cast<float>();
  return rerun::Transform3D{rerun::Vec3D(tr.translation().data()),
                            rerun::Mat3x3(tr.rotation().data())};
}

using pinocchio::FrameIndex;
using BaseVisualizer = pinocchio_visualizers::BaseVisualizer<double>;

class RerunVisualizer : public BaseVisualizer {
public:
  RerunVisualizer(const pinocchio::Model &model,
                  const pinocchio::GeometryModel &geomModel);

  void loadViewerModel() override;

  inline const std::string &prefix() const { return m_prefix; }
  std::string visualPrefix() const { return m_prefix + "/visual"; }

  void drawFrameVelocities(const vector<FrameIndex> &frame_ids);

  void play(const vector<ConstVectorRef> &qs, double dt) override {
    this->play(qs, dt, "trajectory");
  }

  void play(const vector<ConstVectorRef> &qs, double dt,
            const std::string &timeline);

  inline void switchTimeline(const std::string &name) {
    stream.set_time_sequence(name, 0L);
  }
  inline void disableTimeline(const std::string &name) {
    stream.disable_timeline(name);
  }

  rerun::RecordingStream stream;

  inline bool initialized() const { return m_initialized; }

protected:
  void displayImpl() override;
  std::string m_prefix;
  bool m_initialized;
};

} // namespace pinrerun
