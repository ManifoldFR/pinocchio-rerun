#include "./pinocchio.hpp"
#include "./load_mesh.hpp"

namespace pinrerun {

bool loadPinocchioGeometry(const pinocchio::GeometryObject &obj,
                           const rerun::RecordingStream &rr,
                           const std::string &prefix) {
  using namespace hpp::fcl;

  const CollisionGeometry &geom = *obj.geometry;

  switch (geom.getNodeType()) {
  case BV_AABB:
  case BV_OBB:
  case BV_RSS:
  case BV_kIOS:
  case BV_OBBRSS:
  case BV_KDOP16:
  case BV_KDOP18:
  case BV_KDOP24: {
    auto meshDesc = loadMesh(obj.meshPath, obj.meshScale.cast<float>());
    auto rrMesh = meshDescriptionToRerun(std::move(meshDesc));
    rr.log_timeless(getEntityPath(obj, prefix).string(), rrMesh);
    break;
  }
  case GEOM_BOX: {
    break;
  }
  default:
    throw std::logic_error("Unknown or unsupported HPP-FCL node type.");
    break;
  }

  return true;
}

void loadPinocchioModel(const pinocchio::GeometryModel &geomModel,
                        const rerun::RecordingStream &rr,
                        const std::string &prefix) {
  using GeometryObjectVector = pinocchio::GeometryModel::GeometryObjectVector;
  const GeometryObjectVector &geomObjects = geomModel.geometryObjects;
  for (const auto &gobj : geomObjects) {
    loadPinocchioGeometry(gobj, rr, prefix);
  }
}

} // namespace pinrerun
