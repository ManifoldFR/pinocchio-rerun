#include <gtest/gtest.h>
#include <fmt/core.h>

#include "rerun_visualizer.hpp"
#include "./util.hpp"

#include <pinocchio/algorithm/joint-configuration.hpp>

const fs::path ur_descr_path = fs::path(ERD_DIR) / "ur_description";

Robot loadUR(const std::string &robot_name) {
  fs::path urdfPath = ur_descr_path / "urdf" / (robot_name + ".urdf");
  if (!fs::exists(urdfPath))
    throw std::invalid_argument("provided robot " + robot_name +
                                " is not available.");

  return loadRobotFromURDF(urdfPath, package_dirs_base);
}

TEST(LoadRobotTest, load) {

  Robot robot = loadUR("ur3_gripper");
  robot = loadUR("ur5_robot");
}

TEST(LoadRobotTest, UR3_visualizer) {
  Robot robot = loadUR("ur3_gripper");

  fmt::print("Model: {}\n", robot.model.name);
  pinviz::RerunVisualizer rr(robot.model, robot.vizModel);
  rr.stream.set_time_seconds("stable_time", 0.0);
  rr.initViewer();

  Eigen::VectorXd q0 = pinocchio::neutral(robot.model);
  rr.display(q0);
}
