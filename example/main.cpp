#include <algorithm>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "HitboxManager.hpp"

class Window {
 public:
  Window()
    : _window(sf::VideoMode(1280, 1024), "HitboxBuilder") {
    _window.setFramerateLimit(30);
    _window.setKeyRepeatEnabled(false);
  }

  void run() {
    sf::Event event;

    this->loadImages();

    auto tVertices = this->buildPolygon();
    auto bound = this->buildBoundingBox();
    _window.clear(sf::Color(0, 0, 0));
    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
        if (this->handleEvents(event)) {
          tVertices = this->buildPolygon();
          bound = this->buildBoundingBox();
          _window.clear(sf::Color(0, 0, 0));
        }
      }
      _window.draw(_sprites[static_cast<size_t>(_spriteIdx)]);
      for (const auto& v : tVertices) {
        _window.draw(v);
      }
      _window.draw(bound);
      _window.display();
    }
  }

  void loadImages() {
    const std::vector<std::string> files{ "circle", "marioBig", "dog", "Z", "A", "bat", "human", "kraken" };
    _textures.reserve(files.size());

    for (const auto& file : files) {
      sf::Texture texture;

      const auto path = "../example/assets/" + file + ".png";
      if (!texture.loadFromFile(path)) {
        std::cerr << "Could not find the texture '" << path << "'" << std::endl;
        this->close();
        return;
      }
      const auto textureSize = texture.getSize();
      _textures.push_back(std::move(texture));

      sf::Sprite sprite(_textures.back(),
                        sf::IntRect(0, 0, static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)));
      _sprites.push_back(std::move(sprite));
    }
  }

  std::vector<sf::VertexArray> buildPolygon() {
    _hitboxManager.load(_spriteIdx, _sprites[static_cast<size_t>(_spriteIdx)], static_cast<size_t>(_accuracy));
    auto polygons = _hitboxManager.get(_spriteIdx).body();
    std::vector<sf::VertexArray> ppVertices;

    for (size_t i = 0; i < polygons.size(); ++i) {
      const auto& polygon = polygons[i];
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

  sf::VertexArray buildBoundingBox() const {
    const auto boundingBox = _hitboxManager.get(_spriteIdx).bound();
    sf::VertexArray bound(sf::PrimitiveType::LineStrip, 0);

    for (const auto& p : boundingBox) {
      bound.append(sf::Vertex(static_cast<sf::Vector2f>(p)));
    }
    bound.append(sf::Vertex(static_cast<sf::Vector2f>(boundingBox.front())));

    return bound;
  }

  bool handleEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Left) {
        _accuracy = std::max(0, _accuracy - 5);
      } else if (event.key.code == sf::Keyboard::Right) {
        _accuracy = std::min(100, _accuracy + 5);
      } else if (event.key.code == sf::Keyboard::Up) {
        _spriteIdx = std::min(static_cast<int>(_sprites.size()) - 1, _spriteIdx + 1);
      } else if (event.key.code == sf::Keyboard::Down) {
        _spriteIdx = std::max(0, _spriteIdx - 1);
      }
      return event.key.code == sf::Keyboard::Left ||  //
             event.key.code == sf::Keyboard::Right || //
             event.key.code == sf::Keyboard::Up ||    //
             event.key.code == sf::Keyboard::Down;
    }
    return false;
  }

  void close() {
    _window.close();
  }

 private:
  sf::RenderWindow _window;
  Hitbox::Manager<int> _hitboxManager;
  std::vector<sf::Texture> _textures;
  std::vector<sf::Sprite> _sprites;

 private:
  int _accuracy{ 0 };
  int _spriteIdx{ 0 };
};

int main() {
  Window window;

  window.run();
  return 0;
}
