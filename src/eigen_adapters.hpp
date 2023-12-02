#pragma once

#include <rerun.hpp>

#include <Eigen/Core>

namespace {
using rerun::Collection;
}

/// See
/// https://github.com/rerun-io/cpp-example-opencv-eigen/blob/main/src/batch_adapters.hpp
template <>
struct rerun::CollectionAdapter<rerun::Position3D,
                                std::vector<Eigen::Vector3f>> {
  using collection_t = Collection<rerun::Position3D>;
  using vec_t = std::vector<Eigen::Vector3f>;

  collection_t operator()(const vec_t &container) {
    return collection_t::borrow(container.data(), container.size());
  }

  collection_t operator()(vec_t &&container) {
    std::vector<rerun::Position3D> out(container.size());
    auto memsize = container.size() * sizeof(Eigen::Vector3f);
    std::memcpy(out.data(), container.data(), memsize);
    return collection_t::take_ownership(std::move(out));
  }
};

template <>
struct rerun::CollectionAdapter<rerun::Vector3D, std::vector<Eigen::Vector3f>> {
  using collection_t = Collection<rerun::Vector3D>;
  using vec_t = std::vector<Eigen::Vector3f>;

  collection_t operator()(const vec_t &container) {
    return collection_t::borrow(container.data(), container.size());
  }

  collection_t operator()(vec_t &&container) {
    std::vector<rerun::Vector3D> out(container.size());
    auto memsize = container.size() * sizeof(Eigen::Vector3f);
    std::memcpy(out.data(), container.data(), memsize);
    return collection_t::take_ownership(std::move(out));
  }
};
