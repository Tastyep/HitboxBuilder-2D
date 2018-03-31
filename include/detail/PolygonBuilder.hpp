#ifndef HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_POLYGON_BUILDER_HPP

#include <cstddef>
#include <functional>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace HitboxBuilder {
namespace Detail {

class PolygonBuilder {
 public:
  PolygonBuilder();

 private:
  using Contour = std::vector<sf::Vector2i>;

 private:
  const size_t kMinBaseVecLength = 15;
  const size_t kMinVecLength = 8;
  const sf::Vector2i kZeroVector{ 0, 0 };

 public:
  Polygon make(const Contour& contour, size_t accuracy) const;

 private:
  size_t testShortAngle(const Contour& contour, size_t i, const sf::Vector2i& baseDir) const;
  size_t testMedAngle(const Contour& contour, size_t i, const sf::Vector2i& baseDir) const;
  size_t testLongAngle(const Contour& contour, size_t i, const sf::Vector2i& baseDir) const;

  size_t findShortIntersection(const Contour& contour, size_t a, float angle) const;
  size_t findMedIntersection(const Contour& contour, size_t a, const sf::Vector2i& baseVec, float angle) const;
  float computeAngle(const sf::Vector2i& v1, const sf::Vector2i& v2) const;

 private:
  std::vector<std::function<size_t(const Contour&, size_t, const sf::Vector2i&)>> _testers;

  mutable size_t _maxShortAngle;
  mutable size_t _maxMedAngle;
  mutable size_t _maxAngle;

  mutable size_t _prevInter;
  mutable bool _baseVecInit;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
