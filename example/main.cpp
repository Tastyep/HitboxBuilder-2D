#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "ContourBuilder.hpp"
#include "PolygonBuilder.hpp"
#include "Triangulator.hpp"

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

    if (!texture.loadFromFile("../circle.png")) {
      std::cerr << "Could not find the texture '../circle.png'" << std::endl;
      this->close();
      return;
    }

    sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 600, 600 });

    auto contour = _contourBuilder.make(marioSprite);
    sf::VertexArray cVertices(sf::PrimitiveType::LineStrip, 0);
    for (const auto& p : contour) {
      cVertices.append(sf::Vertex(sf::Vector2f{ static_cast<float>(p.x), static_cast<float>(p.y) }));
    }
    cVertices.append(
      sf::Vertex(sf::Vector2f{ static_cast<float>(contour.front().x), static_cast<float>(contour.front().y) }));

    auto polygon = _polygonBuilder.make(contour, _accuracy);
    auto pVertices = this->buildPolygon(polygon);
    auto tVertices = this->buildTriangles(polygon);

    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
        if (this->handleEvents(event)) {
          auto polygon = _polygonBuilder.make(contour, _accuracy);
          pVertices = this->buildPolygon(polygon);
          tVertices = this->buildTriangles(polygon);
          _window.clear();
        }
      }
      _window.draw(marioSprite);
      // _window.draw(cVertices);
      _window.draw(tVertices);
      _window.draw(pVertices);
      _window.display();
    }
  }

  sf::VertexArray buildPolygon(const std::vector<sf::Vector2i>& polygon) {
    sf::VertexArray pVertices(sf::PrimitiveType::LineStrip, 0);
    for (size_t i = 0; i < polygon.size(); ++i) {
      const auto& p = polygon[i];
      pVertices.append(sf::Vertex(static_cast<sf::Vector2f>(p), i % 2 == 0 ? sf::Color::Red : sf::Color::Cyan));
    }
    pVertices.append(
      sf::Vertex(sf::Vector2f{ static_cast<float>(polygon.front().x), static_cast<float>(polygon.front().y) }));
    return pVertices;
  }

  sf::VertexArray buildTriangles(const std::vector<sf::Vector2i>& polygon) {
    auto triangles = _triangulator.convert(polygon);
    sf::VertexArray tVertices(sf::PrimitiveType::LineStrip, 0);

    for (size_t i = 0; i < triangles.size(); ++i) {
      const auto& triangle = triangles[i];

      for (const auto& p : triangle) {
        tVertices.append(sf::Vertex(static_cast<sf::Vector2f>(p)));
      }
      tVertices.append(sf::Vertex(static_cast<sf::Vector2f>(triangle.front())));
    }
    for (const auto& p : triangles.front()) {
      tVertices.append(sf::Vertex(static_cast<sf::Vector2f>(p)));
    }
    tVertices.append(sf::Vertex(static_cast<sf::Vector2f>(triangles.front().front())));

    return tVertices;
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
  HitboxBuilder::Triangulator _triangulator;

 private:
  int _accuracy{ 80 };
};

int main() {
  Window window;

  window.run();
  return 0;
}
