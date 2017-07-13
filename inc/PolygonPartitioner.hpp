#ifndef HITBOX_BUILDER_POLYGON_PARTITIONER_HPP
#define HITBOX_BUILDER_POLYGON_PARTITIONER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Triangulator.hpp"

namespace HitboxBuilder {

class PolygonPartitioner {
 public:
  PolygonPartitioner() = default;

  std::vector<std::vector<sf::Vector2i>> make(std::vector<sf::Vector2i> polygon) const;

 private:
  std::vector<std::vector<sf::Vector2i>> make(std::vector<std::vector<sf::Vector2i>> triangles) const;
  bool isPolygonConvex(const std::vector<sf::Vector2i>& polygon) const;

 private:
  Triangulator _triangulator;
};

} /* namespace HitboxBuilder */

#endif
