#ifndef HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP

#include <tuple>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {
namespace Detail {

class PolygonBuilder {
 private:
  const size_t kMinLength = 4;
  const size_t kMaxUpdateAngle = 20;
  const size_t kShortVecLength = 4;

 public:
  std::vector<sf::Vector2f> make(const std::vector<sf::Vector2i>& contour, size_t accuracy) const;

 private:
  size_t findIntersection(const std::vector<sf::Vector2i>& contour, const sf::Vector2f& baseVec, size_t a,
                          float angle) const;
  size_t pickIntersection(const std::vector<sf::Vector2i>& contour, size_t i) const;
  bool isStrongVariance(const std::vector<sf::Vector2i>& contour, size_t inter) const;
  float computeAngle(const sf::Vector2f& v1, const sf::Vector2f& v2) const;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
