#include "./pinocchio.hpp"
#include "./load_mesh.hpp"
#include <fmt/format.h>

namespace pinrerun {

bool loadPinocchioGeometry(const pinocchio::GeometryObject &obj,
                           const rerun::RecordingStream &rr,
                           const std::string &prefix) {
  using namespace hpp::fcl;

  const CollisionGeometry &geom = *obj.geometry;
  const auto objType = geom.getObjectType();
  const auto nodeType = geom.getNodeType();

  switch (objType) {
  case OT_BVH:
    switch (nodeType) {
    case BV_AABB:
    case BV_OBB:
    case BV_RSS:
    case BV_kIOS:
    case BV_OBBRSS:
    case BV_KDOP16:
    case BV_KDOP18:
    case BV_KDOP24: {
      auto meshDesc = loadMesh(obj.meshPath, obj.meshScale.cast<float>(),
                               obj.meshColor.cast<float>());
      auto rrMesh = meshDescriptionToRerun(std::move(meshDesc));
      rr.log_timeless(getEntityPath(obj, prefix).string(), rrMesh);
      break;
    }
    default:
      fmt::println("Unknown or unsupported HPP-FCL node type {}.",
                   fmt::underlying(nodeType));
      break;
    }
    break;
  default:
    fmt::println("Unknown or unsupported HPP-FCL object type {}.",
                 fmt::underlying(objType));
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
