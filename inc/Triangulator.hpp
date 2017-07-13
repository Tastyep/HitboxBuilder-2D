#ifndef HITBOX_BUILDER_TRIANGULATOR_HPP
#define HITBOX_BUILDER_TRIANGULATOR_HPP

#include <array>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {

class Triangulator {
 private:
  using Triangle = std::array<sf::Vector2i, 3>;

 public:
  Triangulator() = default;

  std::vector<std::array<sf::Vector2i, 3>> convert(std::vector<sf::Vector2i> polygon) const;

 private:
  std::pair<bool, Triangle> nextEar(std::vector<sf::Vector2i>& polygon) const;
  bool isPointContained(const Triangle& triangle, const sf::Vector2i& p) const;

 private:
  bool isTriangleConvex(const Triangle& triangle) const {
    // clang-format off
    return ((triangle[2].x - triangle[0].x) * (triangle[1].y - triangle[0].y) -
            (triangle[2].y - triangle[0].y) * (triangle[1].x - triangle[0].x) >= 0);
    // clang-format on
  }

  bool isPointVertice(const Triangle& triangle, const sf::Vector2i& point) const {
    return (point == triangle[0] || point == triangle[1] || point == triangle[2]);
  }
};

} /* namespace HitboxBuilder */

#endif
