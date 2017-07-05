#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

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

    sf::Sprite marioSprite(texture, sf::IntRect{ 0, 0, 200, 250 });

    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
      }
      _window.draw(marioSprite);
      _window.display();
    }
  }

  void init() {}

  void close() {
    _window.close();
  }

 private:
  sf::RenderWindow _window;
};

int main() {
  Window window;

  window.run();
  return 0;
}
