#ifndef HITBOX_BUILDER_DETAIL_POLYGON_PARTITIONER_HPP
#define HITBOX_BUILDER_DETAIL_POLYGON_PARTITIONER_HPP

#include <cstddef>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Triangulator.hpp"

namespace HitboxBuilder {
namespace Detail {

class PolygonPartitioner {
 private:
  using Triangle = Polygon;

 public:
  std::vector<Polygon> make(Polygon polygon) const;

 private:
  std::vector<Polygon> make(std::vector<Triangle> triangles) const;
  template <typename PolyItB, typename PolyItA>
  std::pair<bool, size_t> findDiagonal(const std::vector<Triangle>& triangles, PolyItB& polygonB,
                                       const PolyItA& polygonA, size_t i) const;
  bool isPolygonConvex(const Polygon& polygon) const;

 private:
  Triangulator _triangulator;

 private:
  mutable size_t _polygonASize;
  mutable size_t _polygonBSize;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
