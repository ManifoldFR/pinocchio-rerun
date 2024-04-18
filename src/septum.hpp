#pragma once

#include "config.hpp"
#include "warning.hpp"
#include "deprecated.hpp"
#include "rerun_visualizer.hpp"

namespace pinrerun {

inline std::string printVersion(const char *delimiter = ".") {
  std::ostringstream oss;
  oss << SEPTUM_MAJOR_VERSION << delimiter << SEPTUM_MINOR_VERSION << delimiter
      << SEPTUM_PATCH_VERSION;
  return oss.str();
}

} // namespace pinrerun
