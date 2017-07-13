#include "Triangulator.hpp"

#include <iostream>
#include <tuple>

namespace HitboxBuilder {

std::vector<std::array<sf::Vector2i, 3>> Triangulator::convert(std::vector<sf::Vector2i> polygon) const {
  std::vector<Triangle> triangles;
  bool success;
  Triangle triangle;

  do {
    std::tie(success, triangle) = this->nextEar(polygon);
    if (success) {
      triangles.push_back(std::move(triangle));
    }
  } while (success);

  return triangles;
}

std::pair<bool, Triangulator::Triangle> Triangulator::nextEar(std::vector<sf::Vector2i>& polygon) const {
  size_t size = polygon.size();
  std::array<sf::Vector2i, 3> triangle;

  if (size < 3) {
    return { false, {} };
  } else if (size == 3) {
    triangle = { polygon[0], polygon[1], polygon[2] };
    polygon.clear();
    return { true, triangle };
  } else {
    for (size_t i = 0; i < size; ++i) {
      bool triTest = false;
      triangle = { polygon[(i + size - 1) % size], polygon[i], polygon[(i + 1) % size] };

      if (this->isTriangleConvex(triangle)) {
        for (const auto& point : polygon) {
          if (this->isPointVertice(triangle, point))
            continue;
          if (this->isPointContained(triangle, point)) {
            triTest = true;
            break;
          }
        }
        if (triTest == false) {
          polygon.erase(std::next(polygon.begin(), i));
          return { true, triangle };
        }
      }
    }
  }
  return { false, {} };
}

bool Triangulator::isPointContained(const Triangle& triangle, const sf::Vector2i& p) const {
  std::array<float, 3> barCoef;
  float eps = 0.0000001;

  barCoef[0] = ((triangle[1].y - triangle[2].y) * (p.x - triangle[2].x) +
                (triangle[2].x - triangle[1].x) * (p.y - triangle[2].y)) /
               (((triangle[1].y - triangle[2].y) * (triangle[0].x - triangle[2].x) +
                 (triangle[2].x - triangle[1].x) * (triangle[0].y - triangle[2].y)) +
                eps);
  barCoef[1] = ((triangle[2].y - triangle[0].y) * (p.x - triangle[2].x) +
                (triangle[0].x - triangle[2].x) * (p.y - triangle[2].y)) /
               (((triangle[1].y - triangle[2].y) * (triangle[0].x - triangle[2].x) +
                 (triangle[2].x - triangle[1].x) * (triangle[0].y - triangle[2].y)) +
                eps);
  barCoef[2] = 1.f - barCoef[0] - barCoef[1];

  for (float coef : barCoef) {
    if (coef >= 1.f || coef <= 0.f)
      return false;
  }
  return true;
}

} /* namespace HitboxBuilder */
