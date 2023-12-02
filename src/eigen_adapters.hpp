#pragma once

#include <rerun.hpp>

#include <Eigen/Core>

/// See
/// https://github.com/rerun-io/cpp-example-opencv-eigen/blob/main/src/batch_adapters.hpp
template <>
struct rerun::ComponentBatchAdapter<rerun::Position3D,
                                    std::vector<Eigen::Vector3f>> {
  using batch_t = rerun::ComponentBatch<rerun::Position3D>;
  using vec_t = std::vector<Eigen::Vector3f>;

  batch_t operator()(const vec_t &container) {
    return batch_t::borrow(container.data(), container.size());
  }

  batch_t operator()(vec_t &&container) {
    std::vector<rerun::Position3D> out(container.size());
    auto memsize = container.size() * sizeof(Eigen::Vector3f);
    std::memcpy(out.data(), container.data(), memsize);
    return batch_t::take_ownership(std::move(out));
  }
};
