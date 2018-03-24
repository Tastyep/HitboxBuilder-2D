#include "detail/Triangulator.hpp"

#include <cmath>
#include <tuple>

namespace HitboxBuilder {
namespace Detail {

std::vector<Polygon> Triangulator::convert(Polygon polygon) const {
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
  Triangle ear;
  bool success;

  std::move(polygon.begin(), polygon.end(), std::back_inserter(vertices));
  polygon.clear();
  this->initVertices(vertices);
  for (auto& vertex : vertices) {
    this->updateVertex(vertices, vertex);
  }

  size_t i = 0;
  size_t nbVertices = vertices.size();
  while ((std::tie(success, ear) = this->nextEar(vertices, nbVertices, i)), success) {
    triangles.push_back(std::move(ear));
    ++i;
  }

  return triangles;
}

void Triangulator::initVertices(std::vector<Vertex>& vertices) const {
  const size_t verticesSize = vertices.size();

  for (size_t i = 0; i < verticesSize; ++i) {
    auto& vertice = vertices[i];

    vertice.prev = &vertices[(i + verticesSize - 1) % verticesSize];
    vertice.next = &vertices[(i + 1) % verticesSize];
  }
}

void Triangulator::updateVertex(const std::vector<Vertex>& vertices, Vertex& vertex) const {
  const Vertex& prev = *vertex.prev;
  const Vertex& next = *vertex.next;
  const auto vPrev = this->normalize(prev.p - vertex.p);
  const auto vNext = this->normalize(next.p - vertex.p);

  vertex.angle = vPrev.x * vNext.x + vPrev.y * vNext.y;
  vertex.isConvex = this->isAngleConvex(prev.p, vertex.p, next.p);
  vertex.isEar = vertex.isConvex;
  if (vertex.isConvex) {
    for (const auto& v : vertices) {
      if (!this->isPointVertice(prev.p, vertex.p, next.p, v.p) && //
          this->isPointContained(prev.p, vertex.p, next.p, v.p)) {
        vertex.isEar = false;
        break;
      }
    }
  }
}

std::pair<bool, Triangulator::Triangle>
Triangulator::nextEar(std::vector<Vertex>& polygon, size_t nbVertices, size_t vertexIndex) const {
  Triangle triangle;
  Vertex* ear = nullptr;

  if (vertexIndex < nbVertices - 3) {
    for (size_t j = 0; j < nbVertices; ++j) {
      auto& v = polygon[j];

      if (!v.isActive || !v.isEar) {
        continue;
      }
      if (ear == nullptr || v.angle > ear->angle) {
        ear = &v;
      }
    }

    if (ear == nullptr) {
      polygon.clear();
      return { false, {} };
    }

    triangle = Triangle{ ear->prev->p, ear->p, ear->next->p };
    ear->isActive = false;
    ear->prev->next = ear->next;
    ear->next->prev = ear->prev;

    if (vertexIndex < nbVertices - 4) {
      this->updateVertex(polygon, *ear->prev);
      this->updateVertex(polygon, *ear->next);
    }
  } else if (vertexIndex == nbVertices - 3) {
    for (size_t i = 0; i < nbVertices; i++) {
      const auto& v = polygon[i];
      if (v.isActive) {
        triangle = Triangle{ v.prev->p, v.p, v.next->p };
        break;
      }
    }
  } else {
    return { false, {} };
  }
  return { true, std::move(triangle) };
}

bool Triangulator::isPointContained(const Point& a, const Point& b, const Point& c, const Point& p) const {
  return !this->isAngleConvex(a, p, b) && //
         !this->isAngleConvex(b, p, c) && //
         !this->isAngleConvex(c, p, a);
}

sf::Vector2f Triangulator::normalize(const Point& p) const {
  float n = std::sqrt(static_cast<float>(p.x * p.x + p.y * p.y));

  return (n == 0) ? sf::Vector2f{ 0, 0 } //
                  : (static_cast<sf::Vector2f>(p) / n);
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
