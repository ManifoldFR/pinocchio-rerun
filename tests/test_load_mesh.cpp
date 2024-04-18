#include <gtest/gtest.h>
#include <fmt/core.h>
#include <rerun.hpp>

#include "./util.hpp"
#include "load_mesh.hpp"

using namespace pinrerun;

const fs::path MAMMOTH_FILE = fs::path(ASSET_DIR) / "mammoth.obj";
MeshDescription mesh;

TEST(LoadMesh, mammoth) {

  fmt::print("Mammoth file: {}\n", MAMMOTH_FILE.c_str());

  mesh = loadMesh(MAMMOTH_FILE);
  fmt::print("Mesh num vertices: {:d}\n", mesh.vertices.size());
  fmt::print("Mesh num normals : {:d}\n", mesh.normals.size());
  fmt::print("Mesh num faces   : {:d}\n", mesh.faceTriangles.size());
}

TEST(LoadMesh, mammoth_rerun) {

  mesh = loadMesh(MAMMOTH_FILE);

  const rerun::RecordingStream rec("mammoth_example_cpp");
  rec.spawn().exit_on_failure();

  rerun::Mesh3D rmesh = meshDescriptionToRerun(std::move(mesh));
  rec.log("mammoth_mesh", std::move(rmesh));
}
