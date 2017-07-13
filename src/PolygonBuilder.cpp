#include "PolygonBuilder.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>

namespace HitboxBuilder {

std::vector<sf::Vector2i> PolygonBuilder::make(const std::vector<sf::Vector2i>& contour, size_t accuracy) const {
  std::vector<sf::Vector2i> polygon;
  sf::Vector2f lvBaseDirection;
  sf::Vector2f svPrevDirection;
  auto first = contour.begin();
  size_t interPoint = 0;
  size_t lvLength = ksvLength;

  accuracy = std::max(0, std::min(100, static_cast<int>(accuracy)));
  const float accRatio = static_cast<float>(100 - accuracy) / 100.f;
  const size_t lvMaxLength = klvMinLength + (klvMaxLength - klvMinLength) * accRatio;
  const size_t lvMaxAngle = klvMinAngle + (klvMaxAngle - klvMinAngle) * accRatio;

  polygon.push_back(*first);
  for (size_t i = ksvLength; i < contour.size(); ++i) {
    const auto& p = contour[i];
    sf::Vector2f lvDirection = static_cast<sf::Vector2f>(p - *first);

    if (lvLength <= ksvLength) {
      if (lvLength == ksvLength) {
        lvBaseDirection = lvDirection;
        svPrevDirection = lvDirection;
      }
      ++lvLength;
      continue;
    }

    const auto& svStart = contour[i - ksvLength];
    sf::Vector2f svDirection{ p - svStart };

    if (svDirection != svPrevDirection) {
      auto inter = this->pickIntersection(contour, i);

      if (this->isStrongVariance(contour, i)) {
        interPoint = inter;
        lvLength = i - interPoint;
      }
    }
    // If no short inter and if the test with the long vector is considered meaningful.
    else if (interPoint == 0 && lvLength == lvMaxLength) {
      const float angle = this->computeAngle(lvBaseDirection, lvDirection);

      if (angle > lvMaxAngle) {
        interPoint = i;
        lvLength = 0;
      }
    }
    if (interPoint != 0) {
      first = std::next(contour.begin(), interPoint);
      polygon.push_back(contour[interPoint]);
      interPoint = 0;
    } else if (lvLength < lvMaxLength) { // If no inter, make the base vector grow.
      ++lvLength;
      lvBaseDirection = lvDirection;
    }
    svPrevDirection = svDirection;
  }
  return polygon;
}

size_t PolygonBuilder::pickIntersection(const std::vector<sf::Vector2i>& contour, size_t i) const {
  sf::Vector2i nextDirection{ contour[i + 1] - contour[i - 1] };

  // Take the outside point.
  if (nextDirection.x == 0 || nextDirection.y == 0) { // Is Straight.
    return i - ksvLength;
  }
  return i - 1;
}

bool PolygonBuilder::isStrongVariance(const std::vector<sf::Vector2i>& contour, size_t inter) const {
  size_t iPrev = (inter + contour.size() - kivLength) % contour.size();
  size_t iNext = (inter + kivLength) % contour.size();

  return this->computeAngle(static_cast<sf::Vector2f>(contour[inter] - contour[iPrev]),
                            static_cast<sf::Vector2f>(contour[iNext] - contour[inter])) > kivMaxAngle;
}

float PolygonBuilder::computeAngle(const sf::Vector2f& v1, const sf::Vector2f& v2) const {
  float dotProduct;
  float norme;

  dotProduct = v1.x * v2.x + v1.y * v2.y;
  norme = std::sqrt(v1.x * v1.x + v1.y * v1.y) * std::sqrt(v2.x * v2.x + v2.y * v2.y);

  return std::acos(dotProduct / norme) * (180.f / M_PI);
}

} /* namespace HitboxBuilder */
