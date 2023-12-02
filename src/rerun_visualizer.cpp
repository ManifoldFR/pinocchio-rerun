#include "rerun_visualizer.hpp"

#include <pinocchio/algorithm/geometry.hpp>

namespace pinviz {

RerunVisualizer::RerunVisualizer(const pinocchio::Model &model,
                                 const pinocchio::GeometryModel &geomModel)
    : stream("RerunVisualizer"), model(model), data(model),
      visualModel(geomModel), visualData(visualModel),
      m_prefix("pin/" + model.name + "/"), m_initialized(false) {
  stream.spawn().exit_on_failure();
  stream.set_time_seconds("stable_time", 0.0);
}

void RerunVisualizer::initViewer() {
  loadPinocchioModel(visualModel, stream, m_prefix);
  m_initialized = true;
}

void RerunVisualizer::display(const std::optional<ConstVectorRef> &q) {
  if (!m_initialized)
    throw std::runtime_error("Visualizer is not initialized.");
  if (q.has_value())
    pinocchio::forwardKinematics(model, data, *q);

  updatePlacements();
}

void RerunVisualizer::updatePlacements() {
  pinocchio::updateGeometryPlacements(model, data, visualModel, visualData);

  const auto &geomObjs = visualModel.geometryObjects;
  long ngeoms = (long)visualModel.ngeoms;

  for (uint i = 0; i < ngeoms; i++) {
    const auto &gobj = geomObjs[i];
    const pinocchio::SE3 &M =
        visualData.oMg[visualModel.getGeometryId(gobj.name)];

    auto path = getEntityPath(gobj, m_prefix).string();
    stream.log(path, pinSE3toRerun(M));
  }
}

} // namespace pinviz
