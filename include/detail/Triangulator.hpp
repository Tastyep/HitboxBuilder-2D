#ifndef HITBOX_BUILDER_DETAIL_TRIANGULATOR_HPP
#define HITBOX_BUILDER_DETAIL_TRIANGULATOR_HPP

#include <stddef.h>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {
namespace Detail {

class Triangulator {
 private:
  using Triangle = std::vector<sf::Vector2f>;

  struct Vertex {
    Vertex(sf::Vector2f&& p)
      : p(std::forward<sf::Vector2f>(p)) {}

    sf::Vector2f p;
    Vertex* prev;
    Vertex* next;
    bool isActive{ true };
    bool isConvex;
    bool isEar;
    float angle;
  };

 public:
  std::vector<std::vector<sf::Vector2f>> convert(std::vector<sf::Vector2f> polygon) const;

 public:
  bool isAngleReflex(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c) const {
    return !this->isAngleConvex(a, b, c);
  }
  bool isAngleConvex(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c) const {
    return (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y) < 0;
  }

 private:
  void initVertices(std::vector<Vertex>& vertices) const;
  void updateVertex(const std::vector<Vertex>& vertices, Vertex& vertex) const;
  std::pair<bool, Triangle> nextEar(std::vector<Vertex>& polygon, size_t nbVertices, size_t verticeIndex) const;
  bool isPointContained(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c,
                        const sf::Vector2f& p) const;
  sf::Vector2f normalize(const sf::Vector2f& p) const;

 private:
  bool isPointVertice(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c,
                      const sf::Vector2f& point) const {
    return (point == a || point == b || point == c);
  }
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
