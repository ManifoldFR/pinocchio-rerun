#include "./pinocchio.hpp"

#include <pinocchio/multibody/geometry.hpp>

namespace pinviz {

auto loadPinGeometry(const pinocchio::GeometryObject &geom) {

  MeshLoaderPtr ml = std::make_shared<hpp::fcl::MeshLoader>();
  hpp::fcl::BVHModelPtr_t p = ml->load(geom.meshPath, geom.meshScale);

  return p;
}

void loadPinocchioModel(const pinocchio::GeometryModel &geomModel,
                        rerun::RecordingStream &rr) {
  using GeometryObjectVector = pinocchio::GeometryModel::GeometryObjectVector;
  const GeometryObjectVector &geomObjects = geomModel.geometryObjects;
  for (const auto &gobj : geomObjects) {
    auto p = loadPinGeometry(gobj);
    auto m = toRerunMesh(p);
    rr.log("model/" + gobj.name, std::move(m));
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
