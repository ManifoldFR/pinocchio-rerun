#pragma once

#include <pinocchio/multibody/model.hpp>
#include <pinocchio/multibody/data.hpp>
#include <hpp/fcl/mesh_loader/loader.h>
#include <hpp/fcl/BVH/BVH_model.h>

#include "eigen_adapters.hpp"
#include "data_types.hpp"

namespace pinocchio {
struct GeometryObject;
}

namespace pinviz {

using hpp::fcl::BVHModelPtr_t;

bool loadPinocchioGeometry(const pinocchio::GeometryObject &geom,
                           const rerun::RecordingStream &rr,
                           const std::string &prefix);

void loadPinocchioModel(const pinocchio::GeometryModel &geomModel,
                        const rerun::RecordingStream &rr,
                        const std::string &prefix = "pin/");

inline auto eigenStdVecCast(const vector<Eigen::Vector3d> &container) {
  vector<Vector3f> out(container.size());
  for (size_t i = 0; i < out.size(); i++) {
    out[i] = container[i].cast<float>();
  }
  return out;
}

rerun::Mesh3D toRerunMesh(const hpp::fcl::BVHModelPtr_t &ptr);

} // namespace pinviz
