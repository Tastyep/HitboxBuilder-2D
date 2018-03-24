#ifndef HITBOX_BUILDER_DETAIL_BOUNDING_BOX_BUILDER_HPP
#define HITBOX_BUILDER_DETAIL_BOUNDING_BOX_BUILDER_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Types.hpp"

namespace HitboxBuilder {
namespace Detail {

class BoundingBoxBuilder {
 public:
  Polygon make(const Polygon& polygon) const;
};

} /* namespace Detail */
} /* namespace HitboxBuilder */

#endif
