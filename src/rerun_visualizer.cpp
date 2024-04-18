#include "rerun_visualizer.hpp"

#include <pinocchio/algorithm/geometry.hpp>
#include <pinocchio/algorithm/frames.hpp>

namespace pinrerun {

RerunVisualizer::RerunVisualizer(const pinocchio::Model &model,
                                 const pinocchio::GeometryModel &geomModel)
    : BaseVisualizer(model, geomModel, nullptr), stream("RerunVisualizer"),
      m_prefix("pinocchio/" + model.name), m_initialized(false) {
  stream.spawn().exit_on_failure();
  stream.set_time_seconds("stable_time", 0.0);
}

void RerunVisualizer::loadViewerModel() {
  loadPinocchioModel(*m_visualModel, stream, visualPrefix());
  m_initialized = true;
}

void RerunVisualizer::displayImpl() {

  const auto &geomObjs = m_visualModel->geometryObjects;
  long ngeoms = (long)m_visualModel->ngeoms;

  for (uint i = 0; i < ngeoms; i++) {
    const auto &gobj = geomObjs[i];
    const pinocchio::SE3 &M =
        visualData.oMg[m_visualModel->getGeometryId(gobj.name)];

    auto path = getEntityPath(gobj, visualPrefix()).string();
    stream.log(path, pinSE3toRerun(M));
  }
}

void RerunVisualizer::drawFrameVelocities(const vector<FrameIndex> &frame_ids) {
  size_t nframes = frame_ids.size();
  vector<Vector3f> frame_vels(nframes);
  vector<Vector3f> frame_pos(nframes);
  vector<std::string> labels(nframes);
  for (size_t i = 0; i < frame_ids.size(); ++i) {
    auto frame_id = frame_ids[i];
    pinocchio::updateFramePlacement(*m_model, data, frame_id);

    auto vel = pinocchio::getFrameVelocity(*m_model, data, frame_id,
                                           pinocchio::LOCAL_WORLD_ALIGNED)
                   .cast<float>();
    frame_vels[i] = vel.linear();
    frame_pos[i] = data.oMf[frame_id].cast<float>().translation();
    labels[i] = m_model->frames[frame_id].name;
  }
  std::string frame_vel_prefix = m_prefix + "/frame_vels";
  stream.log(frame_vel_prefix,
             rerun::Arrows3D::from_vectors(std::move(frame_vels))
                 .with_origins(std::move(frame_pos))
                 .with_labels(std::move(labels)));
}

void RerunVisualizer::play(const vector<ConstVectorRef> &qs, double dt,
                           const std::string &timeline) {
  auto n = qs.size();
  for (size_t i = 0; i < n; ++i) {
    double t = int(i) * dt;
    stream.set_time_seconds(timeline, t);
    display(qs[i]);
  }
}

} // namespace pinrerun
