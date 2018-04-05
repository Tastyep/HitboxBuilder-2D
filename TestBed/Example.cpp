#include "TestBed/Example.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include <SFML/Window/Event.hpp>

#include "Manager.hpp"

namespace TestBed {

Example::Example(const std::vector<std::string>& fonts)
  : _window(sf::VideoMode(1280, 1024), "HitboxBuilder")
  , _center(1280 / 2, 1024 / 2) {
  _window.setFramerateLimit(30);

  _polygonCount.font.loadFromFile(fonts[0]);
  _polygonCount.text.setFont(_polygonCount.font);
  _polygonCount.text.setCharacterSize(40);
  _polygonCount.text.setFillColor(sf::Color::White);
  _polygonCount.text.setPosition(1.3f * _center.x, 1.8f * _center.y);
}

void Example::run(const std::vector<std::string>& images) {
  std::vector<sf::VertexArray> bodyVertices;
  sf::VertexArray boundingBox;
  sf::Event event;

  this->loadSprites(images);
  this->updateDrawables(bodyVertices, boundingBox);
  while (_window.isOpen()) {
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        this->close();
      }
      if (this->handleEvents(event)) {
        this->updateDrawables(bodyVertices, boundingBox);
      }
    }
    _window.clear(sf::Color(0, 0, 0));
    _window.draw(_sprites[_spriteIdx]);
    for (const auto& v : bodyVertices) {
      _window.draw(v);
    }
    _window.draw(boundingBox);
    _window.draw(_polygonCount.text);
    _window.display();
  }
}

void Example::updateDrawables(std::vector<sf::VertexArray>& bodyVertices, sf::VertexArray& boundingBox) {
  bodyVertices = this->buildPolygon();
  boundingBox = this->buildBoundingBox();
  _polygonCount.text.setString("Acc: " + std::to_string(_accuracy) + "%, " + std::to_string(bodyVertices.size()) +
                               " polygons");
}

void Example::loadSprites(const std::vector<std::string>& images) {
  _textures.reserve(images.size());

  for (const auto& image : images) {
    sf::Texture texture;

    if (!texture.loadFromFile(image)) {
      this->close();
      throw std::runtime_error("Could not find the texture '" + image + "'");
    }

    _textures.push_back(std::move(texture));
    sf::Sprite sprite(_textures.back());
    // Move the sprite in the center of the screen.
    const auto spriteSize = sprite.getTextureRect();
    sprite.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
    sprite.setPosition(static_cast<sf::Vector2f>(_center));

    _sprites.push_back(std::move(sprite));
  }
}

std::vector<sf::VertexArray> Example::buildPolygon() {
  _hitboxManager.load(_spriteIdx, _sprites[_spriteIdx], _accuracy, false);

  // Compute the values to center the drawing.
  const auto origin = _sprites[_spriteIdx].getOrigin();
  const auto offset = static_cast<sf::Vector2f>(_center) - origin;

  const auto polygons = _hitboxManager.get(_spriteIdx).body();
  std::vector<sf::VertexArray> bodyVertices;

  // Convert the polygons into drawable ones.
  for (const auto& polygon : polygons) {
    sf::VertexArray poly(sf::PrimitiveType::LineStrip, 0);

    for (size_t j = 0; j < polygon.size(); ++j) {
      auto p = static_cast<sf::Vector2f>(polygon[j]);

      p += offset;
      poly.append(sf::Vertex(p, j % 2 == 0 ? sf::Color::Red : sf::Color::Cyan));
    }
    poly.append(sf::Vertex(static_cast<sf::Vector2f>(polygon.front()) + offset,
                           polygon.size() % 2 == 0 ? sf::Color::Red : sf::Color::Cyan));
    bodyVertices.push_back(std::move(poly));
  }

  return bodyVertices;
}

sf::VertexArray Example::buildBoundingBox() const {
  const auto& boundingBox = _hitboxManager.get(_spriteIdx).bound();
  sf::VertexArray bound(sf::PrimitiveType::LineStrip, 0);

  // Compute the values to center the drawing.
  const auto origin = _sprites[_spriteIdx].getOrigin();
  const auto offset = static_cast<sf::Vector2f>(_center) - origin;

  for (const auto& p : boundingBox) {
    bound.append(sf::Vertex(static_cast<sf::Vector2f>(p) + offset));
  }
  bound.append(sf::Vertex(static_cast<sf::Vector2f>(boundingBox.front()) + offset));

  return bound;
}

bool Example::handleEvents(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Down) {
      _accuracy = static_cast<size_t>(std::max(0, static_cast<int>(_accuracy) - 5));
    } else if (event.key.code == sf::Keyboard::Up) {
      _accuracy = static_cast<size_t>(std::min(100, static_cast<int>(_accuracy) + 5));
    } else if (event.key.code == sf::Keyboard::Right) {
      _spriteIdx =
        static_cast<size_t>(std::min(static_cast<int>(_sprites.size()) - 1, static_cast<int>(_spriteIdx) + 1));
    } else if (event.key.code == sf::Keyboard::Left) {
      _spriteIdx = static_cast<size_t>(std::max(0, static_cast<int>(_spriteIdx) - 1));
    }
    return event.key.code == sf::Keyboard::Left ||  //
           event.key.code == sf::Keyboard::Right || //
           event.key.code == sf::Keyboard::Up ||    //
           event.key.code == sf::Keyboard::Down;
  }
  return false;
}

void Example::close() {
  _window.close();
}

} /* namespace TestBed */
