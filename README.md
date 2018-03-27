<div align="center">
  <img src="https://raw.githubusercontent.com/Tastyep/SFML-HitboxBuilder-2D/develop/assets/images/title.png"  alt="logo">
</div>


## Introduction
Hitbox Builder is a lightweight C++ library aiming to automatizing the process of generating hitboxes.

It was originally designed for 2D games, but it could also serve different purposes.

## License
Hitbox Builder is distributed under the [MIT License](https://raw.githubusercontent.com/Tastyep/SFML-HitboxBuilder-2D/develop/LICENSE)

## Requirements
- [SFML](https://www.sfml-dev.org/)
- [CMake](https://github.com/Kitware/CMake)
- [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/)

## Features
- Contour detection
- Polygon builder
- Polygon triangulation ([Ear clipping](https://en.wikipedia.org/wiki/Two_ears_theorem))
- Polygon partitioning (Hertel-Mehlhorn)

<div align="center">
  <img src="https://raw.githubusercontent.com/Tastyep/SFML-HitboxBuilder-2D/develop/assets/images/banner.png"  alt="logo">
</div>

## Demos

To run the demos, execute the configure script with the option '-t'.

Execute the target HitboxBuilder-tester located in the build directory.

Some test bed commands are:
- <kbd>←</kbd> <kbd>→</kbd> keys to slide between the images.
- <kbd>↑</kbd> <kbd>↓</kbd> keys to increase/decrease the accuracy of the algorithm.

You can load your own images by adding them to the TestBed/assets/ directory. You will just need to restart the program as they are loaded at the beginning.
