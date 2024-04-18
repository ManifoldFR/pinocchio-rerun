#pragma once

#include <Eigen/Core>

namespace pinrerun {

using Eigen::VectorXd;
using VectorRef = Eigen::Ref<VectorXd>;
using ConstVectorRef = Eigen::Ref<const VectorXd>;
using Eigen::Matrix3Xf;
using Eigen::Vector3f;
using Eigen::Vector4f;
using std::vector;

} // namespace pinrerun
