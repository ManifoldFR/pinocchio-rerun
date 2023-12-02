#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "data_types.hpp"

namespace pinviz {

using Vector3u = Eigen::Matrix<uint32_t, 3, 1>;

struct MeshDescription {
  vector<Vector3f> vertices;
  vector<Vector3f> normals;
  vector<Vector3u> faceTriangles;
};

void buildMesh(const aiScene *scene, uint vtxOffset, MeshDescription &mesh);

/// Load mesh vertices, normals, and faces.
MeshDescription loadMesh(const std::string &meshPath);

} // namespace pinviz
