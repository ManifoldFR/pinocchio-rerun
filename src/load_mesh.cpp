#include <fmt/core.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "eigen_adapters.hpp"
#include "load_mesh.hpp"

namespace pinrerun {

namespace internal {

/// @returns Number of vertices
uint buildMesh_impl(const aiScene *scene, const aiNode *node, uint vtxOffset,
                    MeshDescription &mesh, Vector3f scale, Vector4d color) {
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

      vector<uint32_t> c_;
      for (int cId=0; cId<4; ++cId) {
        c_.push_back((uint32_t) (COLOR_FLOAT_TO_INT(color[cId])));
      }
      uint32_t c = PASTEL_RGBA(c_[0], c_[1], c_[2], c_[3]);
      mesh.colors.push_back(c);

      if (inMesh->mNormals) {
        aiVector3D n_ = inMesh->mNormals[vId];
        Eigen::Map<Vector3f> n(&n_.x);
        mesh.normals.push_back(n);
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
    nbVtx += buildMesh_impl(scene, node->mChildren[cIdx], nbVtx, mesh, scale, color);
  }

  return nbVtx;
}

} // namespace internal

void buildMesh(const aiScene *scene, uint vtxOffset, MeshDescription &mesh,
               Vector3f scale, Vector4d color) {
  internal::buildMesh_impl(scene, scene->mRootNode, vtxOffset, mesh, scale, color);
}

MeshDescription loadMesh(const std::string &meshPath, Vector3f scale, Vector4d color) {
  ::Assimp::Importer importer;
  importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                              aiPrimitiveType_LINE | aiPrimitiveType_POINT);
  const aiScene *scene = importer.ReadFile(
      meshPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_GenSmoothNormals | aiProcess_SortByPType |
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
  buildMesh(scene, 0, mesh, scale, color);
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

} // namespace pinrerun
