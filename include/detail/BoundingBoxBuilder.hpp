#ifndef HITBOX_BUILDER_DETAIL_BOUNDING_BOX_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_BOUNDING_BOX_BUILDER_HPP

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {
namespace Detail {

class BoundingBoxBuilder {
 public:
  sf::IntRect make(const std::vector<sf::Vector2i>& polygon) const;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
