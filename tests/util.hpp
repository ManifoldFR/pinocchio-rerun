#include <pinocchio/parsers/urdf.hpp>

#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;

constexpr std::string_view ERD_DIR = EXAMPLE_ROBOT_DATA_MODEL_DIR;
const std::vector<std::string> package_dirs_base = {fs::path(ERD_DIR) /
                                                    "../.."};

struct Robot {
  pinocchio::Model model;
  pinocchio::GeometryModel vizModel;
  pinocchio::GeometryModel collModel;
};

Robot loadRobotFromURDF(const fs::path &urdfPath,
                        const std::vector<std::string> &package_dirs,
                        bool verbose = false);
