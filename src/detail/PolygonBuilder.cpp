#include "detail/PolygonBuilder.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

namespace HitboxBuilder {
namespace Detail {

std::vector<sf::Vector2f> PolygonBuilder::make(const std::vector<sf::Vector2i>& contour, size_t accuracy) const {
  std::vector<sf::Vector2f> polygon;

  const size_t maxShortAngle = 90 - (80 * accuracy) / 100;
  const size_t maxAngle = 60 - (35 * accuracy) / 100;

  auto start = 0;
  auto inter = 0;
  sf::Vector2f baseVec;
  sf::Vector2f longVec;
  sf::Vector2f shortVec;
  float interAngle = 0;

  // Join lines
  polygon.push_back(static_cast<sf::Vector2f>(contour.front()));
  for (size_t i = 0; i < contour.size(); ++i) {
    if (i - start < kMinLength) {
      continue;
    }
    const auto& p = contour[i];

    if (i - start == kMinLength) {
      baseVec = static_cast<sf::Vector2f>(p - contour[start]);
      inter = 0;
      interAngle = 0;
      continue;
    }

    shortVec = static_cast<sf::Vector2f>(p - contour[i - kShortVecLength]);
    longVec = static_cast<sf::Vector2f>(p - contour[start]);
    const auto angle = this->computeAngle(baseVec, longVec);
    const auto shortAngle = this->computeAngle(baseVec, shortVec);
    if (angle == 0) {
      continue;
    }
    // Allow a margin for detecting a change of direction for shapes like circles.
    if ((inter == 0 && angle > kMaxUpdateAngle) || shortAngle >= maxShortAngle) {
      if (shortAngle >= maxShortAngle) {
        inter = this->findIntersection(contour, baseVec, i, shortAngle);
      } else {
        inter = i;
      }
      interAngle = angle;
    } else if (angle < interAngle) {
      inter = i;
      interAngle = angle;
      continue;
    }

    if (angle >= maxAngle || shortAngle >= maxShortAngle) {
      polygon.push_back(static_cast<sf::Vector2f>(contour[inter]));
      start = inter;
      i = start;
    }
  }

  if (inter != 0) {
    polygon.push_back(static_cast<sf::Vector2f>(contour[inter]));
  }

  return polygon;
}

size_t PolygonBuilder::findIntersection(const std::vector<sf::Vector2i>& contour, const sf::Vector2f& baseVec, size_t a,
                                        float angle) const {
  size_t b = a - kShortVecLength;

  for (float maxAngle = angle; b < a; ++b) {
    angle = this->computeAngle(baseVec, static_cast<sf::Vector2f>(contour[a] - contour[b + 1]));
    if (angle <= maxAngle) {
      return b;
    }
    maxAngle = angle;
  }
  return b;
}

float PolygonBuilder::computeAngle(const sf::Vector2f& v1, const sf::Vector2f& v2) const {
  float dotProduct;
  float norme;

  dotProduct = v1.x * v2.x + v1.y * v2.y;
  norme = std::sqrt(v1.x * v1.x + v1.y * v1.y) * std::sqrt(v2.x * v2.x + v2.y * v2.y);

  return std::acos(dotProduct / norme) * (180.f / M_PI);
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
