#include "detail/PolygonBuilder.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

namespace HitboxBuilder {
namespace Detail {

Polygon PolygonBuilder::make(const Contour& contour, size_t accuracy) const {
  Polygon polygon;

  const size_t maxShortAngle = 60 - (50 * accuracy) / 100;
  const size_t maxAngle = 60 - (35 * accuracy) / 100;

  size_t start = 0;
  size_t inter = 0;
  bool baseVecInit = false;
  sf::Vector2i baseVec;
  sf::Vector2i longVec;
  sf::Vector2i shortVec;

  // Join lines
  polygon.push_back(contour.front());
  for (size_t i = 0; i < contour.size(); ++i) {
    if (i - start < kMinLength) {
      continue;
    }

    const auto& p = contour[i];
    if (baseVecInit == false) {
      baseVec = p - contour[start];
      if (baseVec != kZeroVector) {
        baseVecInit = true;
      }
      continue;
    }

    shortVec = p - contour[i - kShortVecLength];
    longVec = p - contour[start];
    const auto angle = this->computeAngle(baseVec, longVec);
    const auto shortAngle = this->computeAngle(baseVec, shortVec);

    if (angle > maxAngle || shortAngle >= maxShortAngle) {
      if (shortAngle >= maxShortAngle) {
        inter = this->findIntersection(contour, baseVec, i, shortAngle);
      } else {
        inter = i;
      }
    }

    if (inter != 0) {
      polygon.push_back(contour[inter]);
      baseVecInit = false;
      start = inter;
      i = inter;
      inter = 0;
    }
  }

  if (inter != 0) {
    polygon.push_back(contour[inter]);
  }

  return polygon;
}

size_t PolygonBuilder::findIntersection(const Contour& contour, const sf::Vector2i& baseVec, size_t a,
                                        float angle) const {
  size_t b = a - kShortVecLength;

  for (float maxAngle = angle; b < a; ++b) {
    angle = this->computeAngle(baseVec, contour[a] - contour[b + 1]);
    if (angle <= maxAngle) {
      return b;
    }
    maxAngle = angle;
  }
  return b;
}

float PolygonBuilder::computeAngle(const sf::Vector2i& v1, const sf::Vector2i& v2) const {
  const auto dotProduct = static_cast<float>(v1.x * v2.x + v1.y * v2.y);
  const auto norme =
    std::sqrt(static_cast<float>(v1.x * v1.x + v1.y * v1.y)) * std::sqrt(static_cast<float>(v2.x * v2.x + v2.y * v2.y));

  return std::acos(dotProduct / norme) * static_cast<float>(180.f / M_PI);
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
