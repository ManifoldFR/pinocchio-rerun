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

using MeshLoaderPtr = std::shared_ptr<hpp::fcl::MeshLoader>;

auto loadPinGeometry(const pinocchio::GeometryObject &geom);

void loadPinocchioModel(const pinocchio::GeometryModel &geomModel,
                        rerun::RecordingStream &rr);

inline auto eigenStdVecCast(const vector<Eigen::Vector3d> &container) {
  vector<Vector3f> out(container.size());
  for (size_t i = 0; i < out.size(); i++) {
    out[i] = container[i].cast<float>();
  }
  return out;
}

rerun::Mesh3D toRerunMesh(const hpp::fcl::BVHModelPtr_t &ptr);

} // namespace pinviz
