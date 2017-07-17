#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "ContourBuilder.hpp"
#include "PolygonBuilder.hpp"
#include "PolygonPartitioner.hpp"

class Window {
 public:
  Window()
    : _window(sf::VideoMode(1280, 1024), "InputConverter") {
    _window.setFramerateLimit(30);
    _window.setKeyRepeatEnabled(false);
  }

  void run() {
    sf::Event event;
    sf::Texture texture;

    if (!texture.loadFromFile("../marioBig.png")) {
      std::cerr << "Could not find the texture '../marioBig.png'" << std::endl;
      this->close();
      return;
    }

    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 600, 600 });
    sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 480, 640 }); // marioBig
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 576, 598 }); // dog
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 640, 739 }); // Z
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 581, 661 }); // A
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 960, 584 }); // bat
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 408, 599 }); // human
    // sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 600, 320 }); // kraken

    auto contour = _contourBuilder.make(marioSprite);
    sf::VertexArray cVertices(sf::PrimitiveType::LineStrip, 0);
    for (const auto& p : contour) {
      cVertices.append(sf::Vertex(sf::Vector2f{ static_cast<float>(p.x), static_cast<float>(p.y) }));
    }
    cVertices.append(
      sf::Vertex(sf::Vector2f{ static_cast<float>(contour.front().x), static_cast<float>(contour.front().y) }));

    auto polygon = _polygonBuilder.make(contour, _accuracy);
    auto pVertices = this->buildPolygon(polygon);
    auto tVertices = this->partitionPolygon(polygon);

    _window.clear(sf::Color(0, 0, 0));
    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
        if (this->handleEvents(event)) {
          auto polygon = _polygonBuilder.make(contour, _accuracy);
          pVertices = this->buildPolygon(polygon);
          tVertices = this->partitionPolygon(polygon);
          _window.clear(sf::Color(0, 0, 0));
        }
      }
      _window.draw(marioSprite);
      // _window.draw(cVertices);
      for (const auto& v : tVertices) {
        _window.draw(v);
      }
      _window.draw(pVertices);
      _window.display();
    }
  }

  sf::VertexArray buildPolygon(const std::vector<sf::Vector2i>& polygon) {
    sf::VertexArray pVertices(sf::PrimitiveType::LineStrip, 0);
    for (size_t i = 0; i < polygon.size(); ++i) {
      const auto& p = polygon[i];
      pVertices.append(sf::Vertex(static_cast<sf::Vector2f>(p), sf::Color::Green));
    }
    pVertices.append(sf::Vertex(static_cast<sf::Vector2f>(polygon.front()), sf::Color::Green));
    return pVertices;
  }

  std::vector<sf::VertexArray> partitionPolygon(const std::vector<sf::Vector2i>& polygon) {
    auto pPolygons = _polygonPartitioner.make(polygon);
    std::vector<sf::VertexArray> ppVertices;

    for (size_t i = 0; i < pPolygons.size(); ++i) {
      const auto& polygon = pPolygons[i];
      sf::VertexArray poly(sf::PrimitiveType::LineStrip, 0);

      for (size_t j = 0; j < polygon.size(); ++j) {
        const auto& p = polygon[j];
        poly.append(sf::Vertex(static_cast<sf::Vector2f>(p), j % 2 == 0 ? sf::Color::Red : sf::Color::Cyan));
      }
      poly.append(sf::Vertex(static_cast<sf::Vector2f>(polygon.front()),
                             polygon.size() % 2 == 0 ? sf::Color::Red : sf::Color::Cyan));
      ppVertices.push_back(std::move(poly));
    }

    return ppVertices;
  }

  bool handleEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Left) {
        _accuracy = std::max(0, _accuracy - 5);
        return true;
      } else if (event.key.code == sf::Keyboard::Right) {
        _accuracy = std::min(100, _accuracy + 5);
        return true;
      }
    }
    return false;
  }

  void close() {
    _window.close();
  }

 private:
  sf::RenderWindow _window;
  HitboxBuilder::ContourBuilder _contourBuilder;
  HitboxBuilder::PolygonBuilder _polygonBuilder;
  HitboxBuilder::PolygonPartitioner _polygonPartitioner;

 private:
  int _accuracy{ 0 };
};

int main() {
  Window window;

  window.run();
  return 0;
}
