#include "detail/Math.hpp"

#include <cmath>

namespace HitboxBuilder {
namespace Detail {
namespace Math {

template <typename T>
float vectorDot(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
  return static_cast<float>(a.x * b.x + a.y * b.y);
}

template <typename T>
float pointDistance(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
  T dx = a.x - b.x;
  T dy = a.y - b.y;

  return static_cast<float>(std::sqrt(dx * dx + dy * dy));
}

template float vectorDot<int>(const sf::Vector2i& a, const sf::Vector2i& b);
template float vectorDot<float>(const sf::Vector2f& a, const sf::Vector2f& b);

template float pointDistance<int>(const sf::Vector2i& a, const sf::Vector2i& b);
template float pointDistance<float>(const sf::Vector2f& a, const sf::Vector2f& b);

} /* namespace Math */
} /* namespace Detail */
} /* namespace HitboxBuilder */
