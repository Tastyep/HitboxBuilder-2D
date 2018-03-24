#include "Manager.hpp"

#include <cassert>
#include <unordered_map>

#include "detail/BoundingBoxBuilder.hpp"
#include "detail/ContourBuilder.hpp"
#include "detail/CoordinateTransformer.hpp"
#include "detail/PolygonBuilder.hpp"
#include "detail/PolygonPartitioner.hpp"

namespace HitboxBuilder {

class Manager::Impl {
 public:
  Impl()
    : _contourBuilder(std::make_unique<Detail::ContourBuilder>())
    , _polygonBuilder(std::make_unique<Detail::PolygonBuilder>())
    , _boundingBoxBuilder(std::make_unique<Detail::BoundingBoxBuilder>())
    , _polygonPartitioner(std::make_unique<Detail::PolygonPartitioner>()) {}

  void load(size_t id, const sf::Sprite& sprite, size_t accuracy, bool isCartesian) {
    auto contour = _contourBuilder->make(sprite);
    auto polygon = _polygonBuilder->make(contour, accuracy);
    auto boundingBox = _boundingBoxBuilder->make(contour);
    auto polygons = _polygonPartitioner->make(std::move(polygon));

    if (isCartesian) {
      Detail::toCarthesian(sprite, polygons);
      Detail::toCarthesian(sprite, boundingBox);
    }
    _hitboxes[id] = Hitbox(std::move(polygons), std::move(boundingBox));
  }

  const Hitbox& get(size_t id) const {
    auto found = _hitboxes.find(id);
    assert(found != _hitboxes.end());

    return found->second;
  }

 private:
  std::unique_ptr<Detail::ContourBuilder> _contourBuilder;
  std::unique_ptr<Detail::PolygonBuilder> _polygonBuilder;
  std::unique_ptr<Detail::BoundingBoxBuilder> _boundingBoxBuilder;
  std::unique_ptr<Detail::PolygonPartitioner> _polygonPartitioner;

 private:
  std::unordered_map<size_t, Hitbox> _hitboxes;
};

Manager::Manager()
  : _impl(std::make_unique<Impl>()) {}

Manager::~Manager() = default;

void Manager::load(size_t id, const sf::Sprite& sprite, size_t accuracy, bool isCartesian) {
  _impl->load(id, sprite, accuracy, isCartesian);
}

const Hitbox& Manager::get(size_t id) const {
  return _impl->get(id);
}

} /* namespace HitboxBuilder */
