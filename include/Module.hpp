#ifndef HITBOX_BUILDER_MODULE_HPP
#define HITBOX_BUILDER_MODULE_HPP

#include <cstddef>

#include <SFML/Graphics/Sprite.hpp>

#include "Types.hpp"

namespace HitboxBuilder {

void init();

Hitbox make(const sf::Sprite& sprite, size_t accuracy, bool isCartesian);


} /* namespace HitboxBuilder */

#endif
