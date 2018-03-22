#ifndef HITBOX_BUILDER_DETAIL_TRIANGULATOR_HPP
#define HITBOX_BUILDER_DETAIL_TRIANGULATOR_HPP

#include <stddef.h>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace Hitbox {
namespace Detail {

class Triangulator {
 private:
  using Triangle = Polygon;
  using Point = sf::Vector2i;

 private:
  struct Vertex {
    Vertex(Point&& p)
      : p(std::forward<Point>(p)) {}

    Point p;
    Vertex* prev;
    Vertex* next;
    bool isActive{ true };
    bool isConvex;
    bool isEar;
    float angle;
  };

 public:
  std::vector<Polygon> convert(Polygon polygon) const;

 public:
  bool isAngleReflex(const Point& a, const Point& b, const Point& c) const {
    return !this->isAngleConvex(a, b, c);
  }
  bool isAngleConvex(const Point& a, const Point& b, const Point& c) const {
    return (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y) < 0;
  }

 private:
  void initVertices(std::vector<Vertex>& vertices) const;
  void updateVertex(const std::vector<Vertex>& vertices, Vertex& vertex) const;
  std::pair<bool, Triangle> nextEar(std::vector<Vertex>& polygon, size_t nbVertices, size_t verticeIndex) const;
  bool isPointContained(const Point& a, const Point& b, const Point& c, const Point& p) const;
  sf::Vector2f normalize(const Point& p) const;

 private:
  bool isPointVertice(const Point& a, const Point& b, const Point& c, const Point& point) const {
    return (point == a || point == b || point == c);
  }
};

} /* namespace Detail */
} /* namespace Hitbox */

#endif
