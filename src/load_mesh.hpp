#pragma once

#include <assimp/Importer.hpp>
#include <rerun/archetypes/mesh3d.hpp>

#include "data_types.hpp"

#define COLOR_FLOAT_TO_INT(f)                                                  \
  ((f >= 1.0 ? 255 : (f <= 0.0 ? 0 : (int)floor(f * 256.0))))
#define PASTEL_RGBA(r, g, b, a)                                                \
  (((r) & 0xFF) << (8 * 3)) | (((g) & 0xFF) << (8 * 2)) |                      \
      (((b) & 0xFF) << (8 * 1)) | (((a) & 0xFF) << (8 * 0));
namespace pinrerun {

using Vector3u = Eigen::Matrix<uint32_t, 3, 1>;

struct MeshDescription {
  vector<Vector3f> vertices;
  vector<Vector3f> normals;
  vector<Vector3u> faceTriangles;
  vector<uint32_t> colors;
};

void buildMesh(const aiScene *scene, uint vtxOffset, MeshDescription &mesh,
               Vector3f scale, Vector4f color);

/// Load mesh vertices, normals, and faces.
MeshDescription loadMesh(const std::string &meshPath,
                         Vector3f scale = Vector3f::Ones(),
                         Vector4f color = Vector4f::Ones());

rerun::archetypes::Mesh3D meshDescriptionToRerun(MeshDescription &&mesh);

} // namespace pinrerun
