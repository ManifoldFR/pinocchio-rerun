#include <gtest/gtest.h>
#include <fmt/core.h>

#include "./util.hpp"
#include "load_mesh.hpp"

using namespace pinviz;

TEST(LoadMesh, mammoth) {

  const fs::path MAMMOTH_FILE = fs::path(ASSET_DIR) / "mammoth.obj";
  fmt::print("Mammoth file: {}\n", MAMMOTH_FILE.c_str());

  MeshDescription mesh = loadMesh(MAMMOTH_FILE);
  fmt::print("Mesh num vertices: {:d}\n", mesh.vertices.size());
  fmt::print("Mesh num normals : {:d}\n", mesh.normals.size());
  fmt::print("Mesh num faces   : {:d}\n", mesh.faceTriangles.size());
}
