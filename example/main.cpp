#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "ContourBuilder.hpp"

class Window {
 public:
  Window()
    : _window(sf::VideoMode(800, 600), "InputConverter") {}

  void run() {
    sf::Event event;
    sf::Texture texture;

    if (!texture.loadFromFile("../mario.png")) {
      std::cerr << "Could not find the texture '../mario.png'" << std::endl;
      this->close();
      return;
    }

    sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 200, 150 });

    auto contour = _contourBuilder.make(marioSprite);
    sf::VertexArray vertices(sf::PrimitiveType::LineStrip, 0);
    for (const auto& p : contour) {
      vertices.append(sf::Vertex(sf::Vector2f{ static_cast<float>(p.x), static_cast<float>(p.y) }));
    }
    vertices.append(
      sf::Vertex(sf::Vector2f{ static_cast<float>(contour.front().x), static_cast<float>(contour.front().y) }));

    for (size_t i = 0; i < vertices.getVertexCount(); ++i) {
      const auto& p = vertices[i].position;
      std::cout << "vertex: " << p.x << ", " << p.y << std::endl;
    }

    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
      }
      _window.draw(marioSprite);
      _window.draw(vertices);
      _window.display();
    }
  }

  void init() {}

  void close() {
    _window.close();
  }

 private:
  sf::RenderWindow _window;
  HitboxBuilder::ContourBuilder _contourBuilder;
};

int main() {
  Window window;

  window.run();
  return 0;
}
