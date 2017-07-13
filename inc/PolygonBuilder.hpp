#ifndef HITBOX_BUILDER_POLYGON_BUILDER_HPP
#define HITBOX_BUILDER_POLYGON_BUILDER_HPP

#include <tuple>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {

class PolygonBuilder {
 private:
  const size_t klvMinLength = 5;
  const size_t klvMaxLength = 12;
  const size_t ksvLength = 2;
  const size_t kivLength = 4;
  const size_t klvMinAngle = 5;
  const size_t klvMaxAngle = 50;
  const size_t kivMaxAngle = 30;

 public:
  std::vector<sf::Vector2i> make(const std::vector<sf::Vector2i>& contour, size_t accuracy) const;

 private:
  size_t pickIntersection(const std::vector<sf::Vector2i>& contour, size_t i) const;
  bool isStrongVariance(const std::vector<sf::Vector2i>& contour, size_t inter) const;
  float computeAngle(const sf::Vector2f& v1, const sf::Vector2f& v2) const;
};

} /* namespace HitboxBuilder */

#endif
