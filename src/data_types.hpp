#pragma once

#include <Eigen/Core>

namespace pinviz {

using Eigen::VectorXd;
using VectorRef = Eigen::Ref<VectorXd>;
using ConstVectorRef = Eigen::Ref<const VectorXd>;
using Eigen::Matrix3Xf;
using Eigen::Vector3f;
using std::vector;

} // namespace pinviz
