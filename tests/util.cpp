#include "./util.hpp"
#include <fmt/core.h>

Robot loadRobotFromURDF(const fs::path &urdfPath,
                        const std::vector<std::string> &package_dirs,
                        bool verbose) {
  if (verbose)
    fmt::print("URDF path is {}\n", urdfPath.c_str());

  pinocchio::Model model;
  pinocchio::GeometryModel vizModel;
  pinocchio::urdf::buildModel(urdfPath, model);
  pinocchio::urdf::buildGeom(model, urdfPath, pinocchio::VISUAL, vizModel,
                             package_dirs);
  return {std::move(model), std::move(vizModel)};
}
