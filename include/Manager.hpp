#ifndef HITBOX_BUILDER_MANAGER_HPP
#define HITBOX_BUILDER_MANAGER_HPP

#include <cstddef>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>

#include "Types.hpp"

namespace HitboxBuilder {

class Manager {
 public:
  Manager();
  ~Manager();
  Manager(const Manager& other) = delete;
  Manager(Manager&& other) = default;
  Manager& operator=(const Manager& other) = delete;
  Manager& operator=(Manager&& other) = default;

  void load(size_t id, const sf::Sprite& sprite, size_t accuracy, bool isCartesian);
  const Hitbox& get(size_t id) const;

 private:
  // Pimpl idiom
  class Impl;
  std::unique_ptr<Impl> _impl;
};

} /* namespace HitboxBuilder */

#endif
