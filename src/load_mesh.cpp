#include <fmt/core.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "eigen_adapters.hpp"
#include "load_mesh.hpp"

namespace pinviz {

namespace internal {

/// @returns Number of vertices
uint buildMesh_impl(const aiScene *scene, const aiNode *node, uint vtxOffset,
                    MeshDescription &mesh, Vector3f scale) {
  if (!node)
    return 0;

  aiMatrix4x4 tr = node->mTransformation;
  aiNode *pNode = node->mParent;

  // traverse through to root, get global transform
  while (pNode) {
    // update transform
    if (pNode->mParent != nullptr) {
      tr = pNode->mTransformation * tr;
    }
    pNode = pNode->mParent;
  }

  uint nbVtx = 0;

  for (uint i = 0; i < node->mNumMeshes; ++i) {
    uint meshIdx = node->mMeshes[i];
    aiMesh *inMesh = scene->mMeshes[meshIdx];

    for (uint vId = 0; vId < inMesh->mNumVertices; vId++) {
      aiVector3D pos_ = inMesh->mVertices[vId];
      pos_ *= tr;
      Eigen::Map<Vector3f> p(&pos_.x);
      mesh.vertices.push_back(scale.asDiagonal() * p);

      if (inMesh->mNormals) {
        aiVector3D n_ = inMesh->mNormals[vId];
        Eigen::Map<Vector3f> n(&n_.x);
        mesh.normals.push_back(n);
      }

      if (inMesh->mColors[0]) {
        aiColor4D c_ = inMesh->mColors[0][vId];
        uint32_t c = PASTEL_RGBA(int(c_.r), int(c_.g), int(c_.b), int(c_.a));
        mesh.colors.push_back(c);
      }
    }

    for (uint fId = 0; fId < inMesh->mNumFaces; fId++) {
      aiFace &face_ = inMesh->mFaces[fId];
      assert(face_.mNumIndices == 3 &&
             "Face number of indices invalid. The aiProcess_Triangulate flag "
             "in Assimp::Importer should fix this.");
      Eigen::Map<Vector3u> faceIdx(face_.mIndices);
      mesh.faceTriangles.push_back(faceIdx.array() + vtxOffset);
    }

    nbVtx += inMesh->mNumVertices;
  }

  for (uint cIdx = 0; cIdx < node->mNumChildren; cIdx++) {
    nbVtx += buildMesh_impl(scene, node->mChildren[cIdx], nbVtx, mesh, scale);
  }

  return nbVtx;
}

} // namespace internal

void buildMesh(const aiScene *scene, uint vtxOffset, MeshDescription &mesh,
               Vector3f scale) {
  internal::buildMesh_impl(scene, scene->mRootNode, vtxOffset, mesh, scale);
}

MeshDescription loadMesh(const std::string &meshPath, Vector3f scale) {
  ::Assimp::Importer importer;
  importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                              aiPrimitiveType_LINE | aiPrimitiveType_POINT);
  const aiScene *scene = importer.ReadFile(
      meshPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_GenNormals | aiProcess_SortByPType |
                    aiProcess_GenUVCoords | aiProcess_OptimizeMeshes |
                    aiProcess_RemoveComponent | aiProcess_FindDegenerates |
                    aiProcess_ImproveCacheLocality);

  if (!scene) {
    auto expMsg =
        fmt::format("Failed to load resource:\n{}", importer.GetErrorString());
    throw std::invalid_argument(expMsg);
  }

  if (!scene->HasMeshes()) {
    auto expMsg = fmt::format("No meshes found in resource file {}.", meshPath);
    throw std::invalid_argument(expMsg);
  }

  MeshDescription mesh;
  buildMesh(scene, 0, mesh, scale);
  return mesh;
}

rerun::archetypes::Mesh3D meshDescriptionToRerun(MeshDescription &&mesh) {
  size_t numTris = mesh.faceTriangles.size();

  vector<uint32_t> indices;
  for (size_t i = 0; i < numTris; i++) {
    Vector3u &tri = mesh.faceTriangles[i];
    indices.push_back(tri[0]);
    indices.push_back(tri[1]);
    indices.push_back(tri[2]);
  }

  auto rmesh = rerun::archetypes::Mesh3D(std::move(mesh.vertices))
                   .with_mesh_properties(
                       rerun::components::MeshProperties(std::move(indices)))
                   .with_vertex_normals(std::move(mesh.normals))
                   .with_vertex_colors(std::move(mesh.colors));
  return rmesh;
}

} // namespace pinviz
