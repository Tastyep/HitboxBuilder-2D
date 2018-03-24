#ifndef HITBOX_BUILDER_TYPES_HPP
#define HITBOX_BUILDER_TYPES_HPP

#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace HitboxBuilder {

using Polygon = std::vector<sf::Vector2i>;

class Hitbox {
 public:
  Hitbox() = default;
  Hitbox(std::vector<Polygon> body, Polygon bound)
    : _body(std::move(body))
    , _bound(std::move(bound)) {}

  const std::vector<Polygon>& body() const {
    return _body;
  }
  const Polygon& bound() const {
    return _bound;
  }

 private:
  std::vector<Polygon> _body;
  Polygon _bound;
};

} /* namespace HitboxBuilder */

#endif
