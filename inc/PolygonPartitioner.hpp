#ifndef HITBOX_BUILDER_POLYGON_PARTITIONER_HPP
#define HITBOX_BUILDER_POLYGON_PARTITIONER_HPP

#include <stddef.h>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Triangulator.hpp"

namespace HitboxBuilder {

class PolygonPartitioner {
 private:
  using Triangle = std::vector<sf::Vector2i>;
  using Polygon = std::vector<sf::Vector2i>;

 public:
  PolygonPartitioner() = default;

  std::vector<std::vector<sf::Vector2i>> make(std::vector<sf::Vector2i> polygon) const;

 private:
  std::vector<Polygon> make(std::vector<Triangle> triangles) const;
  std::pair<bool, size_t> findDiagonal(const auto& triangles, auto& polygonB, const auto& polygonA, size_t i) const;
  bool isPolygonConvex(const Polygon& polygon) const;

 private:
  Triangulator _triangulator;

 private:
  mutable size_t _polygonASize;
  mutable size_t _polygonBSize;
};

} /* namespace HitboxBuilder */

#endif
