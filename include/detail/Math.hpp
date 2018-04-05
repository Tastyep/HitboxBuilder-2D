#ifndef HITBOX_BUILDER_DETAIL_MATH_HPP
#define HITBOX_BUILDER_DETAIL_MATH_HPP

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {
namespace Detail {
namespace Math {

template <typename T>
float vectorDot(const sf::Vector2<T>& a, const sf::Vector2<T>& b);

template <typename T>
float pointDistance(const sf::Vector2<T>& a, const sf::Vector2<T>& b);

} /* namespace Math */
} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
