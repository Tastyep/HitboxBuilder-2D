#include <SFML/Window.hpp>

class Window {
 public:
  Window()
    : _window(sf::VideoMode(800, 600), "InputConverter") {}

  void run() {
    sf::Event event;

    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
      }
    }
  }

  void init() {}

  void close() {
    _window.close();
  }

 private:
  sf::Window _window;
};

int main() {
  Window window;

  window.run();
  return 0;
}
