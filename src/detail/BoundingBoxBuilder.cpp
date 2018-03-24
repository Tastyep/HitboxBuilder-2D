#include "detail/BoundingBoxBuilder.hpp"

#include <algorithm>

namespace HitboxBuilder {
namespace Detail {

Polygon BoundingBoxBuilder::make(const Polygon& polygon) const {
  sf::Vector2i tl = polygon.front(), br = polygon.front();

  for (const auto& v : polygon) {
    tl.x = std::min(tl.x, v.x);
    tl.y = std::min(tl.y, v.y);
    br.x = std::max(br.x, v.x);
    br.y = std::max(br.y, v.y);
  }

  return Polygon{
    sf::Vector2i{ tl.x, tl.y },
    sf::Vector2i{ tl.x, br.y },
    sf::Vector2i{ br.x, br.y },
    sf::Vector2i{ br.x, tl.y },
  };
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
