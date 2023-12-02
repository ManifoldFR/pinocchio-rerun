#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <rerun/archetypes/mesh3d.hpp>

#include "data_types.hpp"

namespace pinviz {

using Vector3u = Eigen::Matrix<uint32_t, 3, 1>;

struct MeshDescription {
  vector<Vector3f> vertices;
  vector<Vector3f> normals;
  vector<Vector3u> faceTriangles;
};

void buildMesh(const aiScene *scene, uint vtxOffset, MeshDescription &mesh,
               Vector3f scale);

/// Load mesh vertices, normals, and faces.
MeshDescription loadMesh(const std::string &meshPath,
                         Vector3f scale = Vector3f::Ones());

rerun::archetypes::Mesh3D meshDescriptionToRerun(MeshDescription &&mesh);

} // namespace pinviz
