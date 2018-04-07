#include "detail/PolygonBuilder.hpp"

#include <algorithm>
#include <cstddef>

#include "detail/Math.hpp"

namespace HitboxBuilder {
namespace Detail {

Polygon PolygonBuilder::make(Contour contour, const Polygon& boundingBox, size_t accuracy) const {
  if (contour.size() <= 3) {
    return contour;
  }

  const auto threshold = this->computeThreshold(boundingBox, accuracy);

  Polygon polygon;
  std::vector<uint8_t> vertices(contour.size() - 1, 1);
  this->fetchFurthestPoints(contour, 0, vertices.size(), vertices, threshold);

  for (size_t i = 0; i < vertices.size(); ++i) {
    if (vertices[i]) {
      polygon.push_back(contour[i]);
    }
  }
  return polygon;
}

float PolygonBuilder::computeThreshold(const Polygon& polygon, size_t accuracy) const {
  const auto h = std::abs(polygon[0].y - polygon[1].y);
  const auto w = std::abs(polygon[1].x - polygon[2].x);
  const auto d = static_cast<float>(std::sqrt(h * h + w * w));
  const auto scaleFactor = std::sqrt(d) / std::sqrt(kAverageDiagonal);
  const auto minDistance = 1.5f * scaleFactor;
  const auto maxDistance = 40.f * scaleFactor;

  return minDistance + maxDistance - (static_cast<float>(accuracy) / 100.f) * maxDistance;
}

void PolygonBuilder::fetchFurthestPoints(const Contour& contour, size_t i, size_t j, std::vector<uint8_t>& vertices,
                                         float threshold) const {
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

    if (maxDistance < threshold) {
      size_t k = i + 1;
      std::fill_n(std::next(vertices.begin(), static_cast<int>(k)), j - k, 0);
      continue;
    }
    indexes.emplace_back(i, maxIndex);
    indexes.emplace_back(maxIndex, j);
  }
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
