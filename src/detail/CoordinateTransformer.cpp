#include "detail/CoordinateTransformer.hpp"

namespace HitboxBuilder {
namespace Detail {

void toCarthesian(const sf::Sprite& sprite, Polygon& polygon) {
  const auto bound = sprite.getLocalBounds();

  std::transform(polygon.begin(), polygon.end(), polygon.begin(), [&bound](auto& v) -> sf::Vector2i {
    return { v.x, -1 * (v.y - static_cast<int>(bound.height)) };
  });
}

void toCarthesian(const sf::Sprite& sprite, std::vector<Polygon>& polygons) {
  for (auto& polygon : polygons) {
    toCarthesian(sprite, polygon);
  }
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
