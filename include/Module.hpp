#ifndef HITBOX_BUILDER_MODULE_HPP
#define HITBOX_BUILDER_MODULE_HPP

#include <cstddef>

#include <SFML/Graphics/Sprite.hpp>

#include "Types.hpp"

namespace HitboxBuilder {

//! Initialize the hitbox builder module.
void init();

//! Factory function that creates a hitbox from the given sprite.
//! @param sprite The sprite to build the hitbox from.
//! @param accuracy a number ranging from 0 to 100.
//! @param isCartesian A boolean value that defines where to put the origin.
//! [bottom, left] if @c true, [top, left] if @c false.
//! @return The hitbox matching the sprite.
Hitbox make(const sf::Sprite& sprite, size_t accuracy, bool isCartesian);

} /* namespace HitboxBuilder */

#endif
