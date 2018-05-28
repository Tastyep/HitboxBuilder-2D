#include "Module.hpp"

#include <cassert>
#include <memory>

#include "detail/BoundingBoxBuilder.hpp"
#include "detail/ContourBuilder.hpp"
#include "detail/CoordinateTransformer.hpp"
#include "detail/PolygonBuilder.hpp"
#include "detail/PolygonPartitioner.hpp"

namespace HitboxBuilder {
namespace {

std::unique_ptr<Detail::ContourBuilder> contourBuilder;
std::unique_ptr<Detail::PolygonBuilder> polygonBuilder;
std::unique_ptr<Detail::BoundingBoxBuilder> boundingBoxBuilder;
std::unique_ptr<Detail::PolygonPartitioner> polygonPartitioner;
bool init_called{false};

} /* namespace */

void init() {
  contourBuilder = std::make_unique<Detail::ContourBuilder>();
  polygonBuilder = std::make_unique<Detail::PolygonBuilder>();
  boundingBoxBuilder = std::make_unique<Detail::BoundingBoxBuilder>();
  polygonPartitioner = std::make_unique<Detail::PolygonPartitioner>();
  init_called = true;
}

Hitbox make(const sf::Sprite& sprite, size_t accuracy, bool isCartesian) {
  assert(init_called);

  auto contour = contourBuilder->make(sprite, accuracy);
  auto boundingBox = boundingBoxBuilder->make(contour);
  auto polygon = polygonBuilder->make(std::move(contour), boundingBox, accuracy);
  auto polygons = polygonPartitioner->make(std::move(polygon));

  if (isCartesian) {
    Detail::toCarthesian(sprite, polygons);
    Detail::toCarthesian(sprite, boundingBox);
  }

  return Hitbox(std::move(polygons), std::move(boundingBox));
}

} /* namespace HitboxBuilder */
