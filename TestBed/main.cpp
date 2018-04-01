#include <stdexcept>
#include <string>

#include "TestBed/Example.hpp"
#include "TestBed/FileSystem.hpp"

int main() {
  const auto fontPath = std::string("../TestBed/assets/fonts/");
  const auto images = TestBed::readFiles("../TestBed/assets/images/", ".png");
  const auto fonts = TestBed::readFiles(fontPath, ".ttf");

  if (fonts.empty()) {
    throw std::runtime_error("No font found in '" + fontPath + "'");
  }
  TestBed::Example example(fonts);

  example.run(images);
  return 0;
}
