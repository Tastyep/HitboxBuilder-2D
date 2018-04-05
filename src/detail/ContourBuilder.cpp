#include "detail/ContourBuilder.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <iostream>

namespace HitboxBuilder {
namespace Detail {

std::vector<sf::Vector2i> ContourBuilder::make(const sf::Sprite& sprite, size_t accuracy) const {
  const sf::Texture* texture = sprite.getTexture();
  sf::Image image = texture->copyToImage();

  _alphaThreshold = static_cast<uint8_t>(10 + 4 * (accuracy / 10));
  _bound = sprite.getTextureRect();
  _image = &image;

  auto start = this->findStartPoint();
  return this->walkPerimeter(start);
}

sf::Vector2i ContourBuilder::findStartPoint() const {
  sf::Vector2i pos;

  for (pos.y = 0; pos.y < _bound.height - 1; ++pos.y) {
    for (pos.x = 0; pos.x < _bound.width - 1; ++pos.x) {
      if (this->isPixelSolid(pos.x, pos.y) && //
          !this->isPixelSolid(pos.x, pos.y - 1)) {
        --pos.y;
        return pos;
      }
      if (this->isPixelSolid(pos.x + 1, pos.y)) {
        return pos;
      }
    }
  }
  return { 0, 0 };
}

std::vector<sf::Vector2i> ContourBuilder::walkPerimeter(const sf::Vector2i& start) const {
  std::vector<sf::Vector2i> contour;
  StepDirection nextStep{ StepDirection::None };
  sf::Vector2i point = start;

  do {
    this->step(contour, point, nextStep);

    // clang-format off
    switch (nextStep) {
    case StepDirection::N:
      --point.y;
      break;
    case StepDirection::W:
      --point.x;
      break;
    case StepDirection::S:
      ++point.y;
      break;
    case StepDirection::E:
      ++point.x;
      break;
    default:
      return contour;
    }
    // clang-format on
  } while (point != start);

  return contour;
}

void ContourBuilder::step(std::vector<sf::Vector2i>& contour,
                          const sf::Vector2i& point,
                          StepDirection& nextStep) const {
  StepDirection cuStep = nextStep;
  const auto state = this->pixelState(point);

  this->savePoint(contour, point);
  // clang-format off
  switch (state) {
  case 6:
    nextStep = (cuStep == StepDirection::N ? StepDirection::W
                                           : StepDirection::E);
    break;
  case 9:
    nextStep = (cuStep == StepDirection::E ? StepDirection::N
                                           : StepDirection::S);
    break;
  default:
    nextStep = this->directions[state];
  }
  // clang-format on
}

void ContourBuilder::savePoint(std::vector<sf::Vector2i>& contour, const sf::Vector2i& p) const {
  if (this->isPixelSolid(p)) {
    if (contour.empty() || contour.back() != p) {
      contour.push_back(p);
    }
  } else {
    sf::Vector2i pr{ p.x + 1, p.y };
    if (this->isPixelSolid(pr)) {
      if (contour.empty() || (contour.back() != pr && contour.front() != pr)) {
        contour.push_back(pr);
      }
    }

    sf::Vector2i pu{ p.x, p.y + 1 };
    if (this->isPixelSolid(pu)) {
      if (contour.empty() || (contour.back() != pu && contour.front() != pu)) {
        contour.push_back(pu);
      }
    }
  }
}

size_t ContourBuilder::pixelState(const sf::Vector2i& p) const {
  bool upLeft = this->isPixelSolid(p.x, p.y);
  bool upRight = this->isPixelSolid(p.x + 1, p.y);
  bool downRight = this->isPixelSolid(p.x + 1, p.y + 1);
  bool downLeft = this->isPixelSolid(p.x, p.y + 1);

  return static_cast<size_t>(upLeft | upRight << 1 | downRight << 3 | downLeft << 2);
}

bool ContourBuilder::isPixelSolid(int x, int y) const {
  if (x < 0 || y < 0 || x >= _bound.width || y >= _bound.height) {
    return false;
  }

  return _image->getPixel(static_cast<unsigned>(x + _bound.left), static_cast<unsigned>(y + _bound.top)).a >
         _alphaThreshold;
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
