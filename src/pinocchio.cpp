#include "./pinocchio.hpp"

#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

bool loadPinocchioGeometry(const pinocchio::GeometryObject &obj,
                           const rerun::RecordingStream &rr,
                           const std::string &prefix) {
  using namespace hpp::fcl;

  const CollisionGeometry &geom = *obj.geometry;
  std::cout << "Loading geometry " << obj.name
            << ", meshPath = " << obj.meshPath
            << ", objectType = " << geom.getObjectType()
            << ", nodeType = " << geom.getNodeType() << std::endl;

  BVHModelPtr_t p;

  switch (geom.getNodeType()) {
  case BV_AABB:
  case BV_OBB:
  case BV_RSS:
  case BV_kIOS:
  case BV_OBBRSS:
  case BV_KDOP16:
  case BV_KDOP18:
  case BV_KDOP24: {
    shared_ptr<MeshLoader> ml = std::make_shared<MeshLoader>();
    auto p = ml->load(obj.meshPath, obj.meshScale);
    auto mesh = toRerunMesh(p);
    rr.log(prefix + obj.name, mesh);
    break;
  }
  case GEOM_BOX: {
    break;
  }
  default:
    // throw std::logic_error("Unknown HPP-FCL node type.");
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

rerun::Mesh3D toRerunMesh(const hpp::fcl::BVHModelPtr_t &ptr) {
  uint numTris = ptr->num_tris;
  uint numVtx = ptr->num_vertices;

  // convert down to float
  vector<Vector3f> vtx = eigenStdVecCast(*ptr->vertices);
  return rerun::Mesh3D(vtx);
}

} // namespace pinviz
