#include "PolygonPartitioner.hpp"

#include <iostream>
#include <stddef.h>

namespace HitboxBuilder {

std::vector<std::vector<sf::Vector2i>> PolygonPartitioner::make(std::vector<sf::Vector2i> polygon) const {
  if (this->isPolygonConvex(polygon)) {
    return { polygon };
  }
  auto triangles = _triangulator.convert(polygon);
  auto polygons = this->make(std::move(triangles));

  return polygons;
}

std::vector<std::vector<sf::Vector2i>>
PolygonPartitioner::make(std::vector<std::vector<sf::Vector2i>> triangles) const {
  return triangles;
}

bool PolygonPartitioner::isPolygonConvex(const std::vector<sf::Vector2i>& polygon) const {
  size_t i;
  size_t count = polygon.size();

  for (i = 0; i < count; ++i) {
    size_t prev = (i + count - 1) % count;
    size_t next = (i + 1) % count;
    if (!_triangulator.isAngleReflex(polygon[prev], polygon[i], polygon[next])) {
      break;
    }
  }

  return i == count;
}

} /* namespace HitboxBuilder */
