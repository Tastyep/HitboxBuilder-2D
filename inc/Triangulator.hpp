#ifndef HITBOX_BUILDER_TRIANGULATOR_HPP
#define HITBOX_BUILDER_TRIANGULATOR_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {

class Triangulator {
 private:
  using Triangle = std::vector<sf::Vector2i>;

 public:
  Triangulator() = default;

  std::vector<std::vector<sf::Vector2i>> convert(std::vector<sf::Vector2i> polygon) const;

 public:
  bool isAngleReflex(const sf::Vector2i& a, const sf::Vector2i& b, const sf::Vector2i& c) const {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0;
  }

 private:
  std::pair<bool, Triangle> nextEar(std::vector<sf::Vector2i>& polygon) const;
  bool isPointContained(const Triangle& triangle, const sf::Vector2i& p) const;

 private:
  bool isPointVertice(const Triangle& triangle, const sf::Vector2i& point) const {
    return (point == triangle[0] || point == triangle[1] || point == triangle[2]);
  }
  bool isAngleReflex(const Triangle& triangle) const {
    return this->isAngleReflex(triangle[0], triangle[1], triangle[2]);
  }
};

} /* namespace HitboxBuilder */

#endif
