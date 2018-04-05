#ifndef HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP

#include <cstddef>
#include <functional>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace HitboxBuilder {
namespace Detail {

class PolygonBuilder {
 private:
  using Contour = std::vector<sf::Vector2i>;

 public:
  Polygon make(const Contour& contour, size_t accuracy) const;

 private:
  void fetchFurthestPoints(const Contour& contour, size_t i, size_t j, std::vector<uint8_t>& vertices,
                           float threshold) const;
  float pointSegmentLineDistance(const sf::Vector2i& p, const sf::Vector2i& a, const sf::Vector2i& b) const;
};

} /* namespace Detail */
} // namespace HitboxBuilder

#endif
