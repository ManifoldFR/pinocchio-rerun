#include "rerun_visualizer.hpp"

#include <pinocchio/algorithm/geometry.hpp>

namespace pinviz {

RerunVisualizer::RerunVisualizer(const pinocchio::Model &model,
                                 const pinocchio::GeometryModel &geomModel)
    : stream("RerunVisualizer"), model(model), data(model),
      visualModel(geomModel), visualData(visualModel) {
  auto e = stream.spawn();
  e.exit_on_failure();
  prefix = "pin/" + model.name + "/";
  stream.set_time_seconds("stable_time", 0.0);
}

void RerunVisualizer::initViewer() const {
  loadPinocchioModel(visualModel, stream, prefix);
}

void RerunVisualizer::display(ConstVectorRef const &q) {
  pinocchio::forwardKinematics(model, data, q);

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

    auto path = getEntityPath(gobj, prefix).string();
    stream.log(path, pinSE3toRerun(M));
  }
}

} // namespace pinviz
