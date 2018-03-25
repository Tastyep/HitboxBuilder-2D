#include "TestBed/Example.hpp"
#include "TestBed/FileSystem.hpp"

int main() {
  TestBed::Example example;
  const auto images = TestBed::readImages("../TestBed/assets/", ".png");

  example.run(images);
  return 0;
}
