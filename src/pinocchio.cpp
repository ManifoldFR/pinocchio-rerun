#include "./pinocchio.hpp"

namespace pinviz {

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
    shared_ptr<MeshLoader> ml =
        std::make_shared<MeshLoader>(geom.getNodeType());
    auto mesh = loadMesh(obj, ml);
    rr.log(getEntityPath(obj, prefix).string(), mesh);
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

rerun::Mesh3D loadMesh(const pinocchio::GeometryObject &obj,
                       const std::shared_ptr<hpp::fcl::MeshLoader> &ml) {
  auto ptr = ml->load(obj.meshPath, obj.meshScale);
  uint numTris = ptr->num_tris;
  uint numVtx = ptr->num_vertices;

  const vector<hpp::fcl::Triangle> &triangles = *ptr->tri_indices;

  // convert down to float
  vector<Vector3f> vertex_positions = eigenStdVecCast(*ptr->vertices);

  vector<uint32_t> indices;
  for (uint i = 0; i < ptr->num_tris; i++) {
    hpp::fcl::Triangle const &tri = triangles[i];
    indices.push_back(tri[0]);
    indices.push_back(tri[1]);
    indices.push_back(tri[2]);
  }

  const Eigen::Matrix<uint8_t, 4, 1> colAs32 =
      (obj.meshColor * 256).cast<uint8_t>();
  rerun::datatypes::Rgba32 rgba(colAs32[0], colAs32[1], colAs32[2], colAs32[3]);
  vector<rerun::Color> vertex_colors(numVtx, rgba);

  auto props = rerun::MeshProperties(indices);
  std::cout << "num mesh vtx: " << numVtx << std::endl;
  return rerun::Mesh3D(vertex_positions)
      .with_mesh_properties(props)
      .with_vertex_colors(vertex_colors);
}

} // namespace pinviz
