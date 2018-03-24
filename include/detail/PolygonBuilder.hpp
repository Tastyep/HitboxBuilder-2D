#ifndef HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP

#include <cstddef>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace HitboxBuilder {
namespace Detail {

class PolygonBuilder {
 private:
  using Contour = std::vector<sf::Vector2i>;

 private:
  const size_t kMinLength = 4;
  const size_t kMaxUpdateAngle = 20;
  const size_t kShortVecLength = 4;

 public:
  Polygon make(const Contour& contour, size_t accuracy) const;

 private:
  size_t findIntersection(const Contour& contour, const sf::Vector2i& baseVec, size_t a, float angle) const;
  float computeAngle(const sf::Vector2i& v1, const sf::Vector2i& v2) const;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
