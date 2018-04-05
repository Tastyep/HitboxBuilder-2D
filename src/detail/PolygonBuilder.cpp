#include "detail/PolygonBuilder.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>

#include "detail/Math.hpp"

namespace HitboxBuilder {
namespace Detail {

PolygonBuilder::PolygonBuilder() {
  _testers.push_back([this](const Contour& contour, size_t i, const sf::Vector2i& baseDir) {
    return this->testShortAngle(contour, i, baseDir);
  });
  _testers.push_back([this](const Contour& contour, size_t i, const sf::Vector2i& baseDir) {
    return this->testMedAngle(contour, i, baseDir);
  });
  _testers.push_back([this](const Contour& contour, size_t i, const sf::Vector2i& baseDir) {
    return this->testLongAngle(contour, i, baseDir);
  });
}

Polygon PolygonBuilder::make(const Contour& contour, size_t accuracy) const {
  Polygon polygon;

  // The max short angle should only be 80 (and not 90) as one pixel could be detected int eh angle.
  // This non visible pixel would reduce the size of the angle and approximate the result.
  _maxShortAngle = 80 - (50 * accuracy) / 100;
  _maxMedAngle = 80 - (70 * accuracy) / 100;
  _maxAngle = 45 - (40 * accuracy) / 100;

  size_t inter = 0;
  sf::Vector2i baseVec;
  sf::Vector2i longVec;
  sf::Vector2i medVec;
  sf::Vector2i shortVec;

  _baseVecInit = false;
  _prevInter = 0;

  polygon.push_back(contour.front());
  for (size_t i = 0; i < contour.size(); ++i) {
    if (i - _prevInter < kMinVecLength) {
      continue;
    }

    if (i - _prevInter >= kMinBaseVecLength && _baseVecInit == false) {
      baseVec = contour[i] - contour[_prevInter];
      if (baseVec != kZeroVector) {
        _baseVecInit = true;
      }
      continue;
    }

    for (size_t j = 0; j < _testers.size() && inter == 0; ++j) {
      inter = _testers[j](contour, i, baseVec);
    }

    if (inter != 0) {
      polygon.push_back(contour[inter]);
      _baseVecInit = false;
      _prevInter = inter;
      i = inter;
      inter = 0;
    }
  }

  if (inter != 0) {
    polygon.push_back(contour[inter]);
  }
  return polygon;
}

Polygon PolygonBuilder::make2(const Contour& contour, size_t accuracy) const {
  if (contour.size() <= 3) {
    return contour;
  }
  Polygon polygon;

  _maxDistance = 2.f + 5.f * accuracy / 10;
  std::vector<uint8_t> vertices(contour.size(), 1);

  this->fetchFurthestPoint(contour, 0, contour.size() - 1, vertices);

  for (size_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i]) {
      polygon.push_back(contour[i]);
    }
  }
  return polygon;
}

void PolygonBuilder::fetchFurthestPoint(const Contour& contour, size_t i, size_t j,
                                        std::vector<uint8_t>& vertices) const {
  std::vector<std::pair<size_t, size_t>> indexes;

  indexes.reserve(contour.size());
  indexes.emplace_back(i, j);
  while (!indexes.empty()) {
    std::tie(i, j) = indexes.back();
    indexes.pop_back();
    const auto& a = contour[i];
    const auto& b = contour[j];
    float maxDistance = 0.f;
    size_t maxIndex = i;

    for (size_t k = i + 1; k < j; ++k) {
      const auto& p = contour[k];
      const auto distance = this->pointSegmentLineDistance(p, a, b);

      if (distance >= maxDistance) {
        maxDistance = distance;
        maxIndex = k;
      }
    }

    if (maxDistance < _maxDistance) {
      size_t k = i + 1;
      std::fill_n(std::next(vertices.begin(), static_cast<int>(k)), j - k, 0);
      continue;
    }
    indexes.emplace_back(i, maxIndex);
    indexes.emplace_back(maxIndex, j);
  }
}

size_t PolygonBuilder::testShortAngle(const Contour& contour, size_t i, const sf::Vector2i&) const {
  const auto prevDir = contour[i] - contour[i - kMinVecLength];
  const auto nextDir = contour[(i + kMinVecLength) % contour.size()] - contour[i];
  const auto angle = this->computeAngle(prevDir, nextDir);

  return angle >= _maxShortAngle ? this->findShortIntersection(contour, i, angle) : 0;
}

size_t PolygonBuilder::testMedAngle(const Contour& contour, size_t i, const sf::Vector2i& baseDir) const {
  if (_baseVecInit == false) {
    return 0;
  }
  const auto currentDir = contour[i] - contour[i - kMinVecLength];
  const auto angle = this->computeAngle(baseDir, currentDir);

  return angle >= _maxMedAngle ? i : 0;
}

size_t PolygonBuilder::testLongAngle(const Contour& contour, size_t i, const sf::Vector2i& baseDir) const {
  if (_baseVecInit == false) {
    return 0;
  }
  const auto currentDir = contour[i] - contour[_prevInter];
  const auto angle = this->computeAngle(baseDir, currentDir);

  return angle >= _maxAngle ? i : 0;
}

size_t PolygonBuilder::findShortIntersection(const Contour& contour, size_t i, float angle) const {
  // Avoid making segments of length < kMinVecLength.
  // An intersection detected at i >= contour.size() would be problematic for breaking out of the main loop.
  const size_t head = std::min(i + kMinVecLength, contour.size() - kMinVecLength - 1);

  for (float maxAngle = angle; i < head; ++i) {
    const auto j = i + 1;
    const auto prevDir = contour[j] - contour[j - kMinVecLength];
    const auto nextDir = contour[j + kMinVecLength] - contour[j];

    angle = this->computeAngle(prevDir, nextDir);
    if (angle < maxAngle) {
      return i;
    }
    maxAngle = angle;
  }
  return i;
}

float PolygonBuilder::computeAngle(const sf::Vector2i& v1, const sf::Vector2i& v2) const {
  const auto dotProduct = static_cast<float>(v1.x * v2.x + v1.y * v2.y);
  const auto norme =
    std::sqrt(static_cast<float>(v1.x * v1.x + v1.y * v1.y)) * std::sqrt(static_cast<float>(v2.x * v2.x + v2.y * v2.y));

  return std::acos(dotProduct / norme) * static_cast<float>(180.f / M_PI);
}

float PolygonBuilder::pointSegmentLineDistance(const sf::Vector2i& p, const sf::Vector2i& a,
                                               const sf::Vector2i& b) const {
  if (a == b) {
    return Math::pointDistance(p, a);
  }

  const auto v = b - a;
  const auto w = p - a;
  float c1 = Math::vectorDot(w, v);
  if (c1 <= 0) {
    return Math::pointDistance(p, a);
  }

  float c2 = Math::vectorDot(v, v);
  if (c2 <= c1) {
    return Math::pointDistance(p, b);
  }

  float t = c1 / c2;
  const auto shiftedPoint = static_cast<sf::Vector2f>(a) + (static_cast<sf::Vector2f>(v) * t);
  return Math::pointDistance(static_cast<sf::Vector2f>(p), shiftedPoint);
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
