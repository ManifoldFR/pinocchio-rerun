#pragma once

#include "config.hpp"
#include "warning.hpp"
#include "deprecated.hpp"
#include "rerun_visualizer.hpp"

namespace pinrerun {

inline std::string printVersion(const char *delimiter = ".") {
  std::ostringstream oss;
  oss << PINOCCHIO_RERUN_MAJOR_VERSION << delimiter
      << PINOCCHIO_RERUN_MINOR_VERSION << delimiter
      << PINOCCHIO_RERUN_PATCH_VERSION;
  return oss.str();
}

} // namespace pinrerun
