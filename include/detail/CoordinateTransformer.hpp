#ifndef HITBOX_BUILDER_DETAIL_COORDINATE_TRANSFORMER_HPP
#define HITBOX_BUILDER_DETAIL_COORDINATE_TRANSFORMER_HPP

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace Hitbox {
namespace Detail {

void toCarthesian(const sf::Sprite& sprite, Polygon& polygon);
void toCarthesian(const sf::Sprite& sprite, std::vector<Polygon>& polygons);

} /* namespace Detail */
} /* namespace Hitbox */

#endif
