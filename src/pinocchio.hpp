#pragma once

#include <pinocchio/multibody/model.hpp>
#include <pinocchio/multibody/data.hpp>
#include <pinocchio/multibody/geometry.hpp>

#include <filesystem>

#include "eigen_adapters.hpp"
#include "data_types.hpp"

namespace pinrerun {

namespace fs = std::filesystem;

inline fs::path getEntityPath(const pinocchio::GeometryObject &gobj,
                              const std::string &prefix) {
  return fs::path(prefix) / gobj.name;
}

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

} // namespace pinrerun
