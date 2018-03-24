#include "detail/PolygonPartitioner.hpp"

#include <tuple>

namespace HitboxBuilder {
namespace Detail {

std::vector<Polygon> PolygonPartitioner::make(Polygon polygon) const {
  if (this->isPolygonConvex(polygon)) {
    return { polygon };
  }

  auto triangles = _triangulator.convert(polygon);
  auto polygons = this->make(std::move(triangles));

  return polygons;
}

std::vector<Polygon> PolygonPartitioner::make(std::vector<Triangle> triangles) const {
  for (auto polygonA = triangles.begin(); polygonA != triangles.end(); ++polygonA) {
    _polygonASize = polygonA->size();

    for (int verticeIdx = 0; static_cast<size_t>(verticeIdx) < _polygonASize; ++verticeIdx) {
      decltype(polygonA) polygonB;
      bool diagonalFound;
      const auto i = static_cast<size_t>(verticeIdx); // Make a copy to cast only once.
      size_t j;

      std::tie(diagonalFound, j) = this->findDiagonal(triangles, polygonB, polygonA, i);
      if (diagonalFound == false) {
        continue;
      }
      size_t i2 = (i + 1) % _polygonASize;                 // End of the diagonal for polygonA
      size_t i0 = (i + _polygonASize - 1) % _polygonASize; // Point preceding the diagonal for polygonA
      size_t j2 = (j + 1) % _polygonBSize;                 // End of the diagonal for polygonB
      size_t j3 = (j2 + 1) % _polygonBSize;                // Point after the diagonal for polygonB

      // Check the left side of the diagonal
      if (_triangulator.isAngleReflex((*polygonA)[i0], (*polygonA)[i], (*polygonB)[j3])) {
        continue;
      }

      size_t i3 = (i2 + 1) % _polygonASize;                // Point after the diagonal for polygonA
      size_t j0 = (j + _polygonBSize - 1) % _polygonBSize; // Point preceding the diagonal for polygonB

      // Check the right side of the diagonal
      if (_triangulator.isAngleReflex((*polygonB)[j0], (*polygonA)[i2], (*polygonA)[i3])) {
        continue;
      }

      Polygon polygon;

      polygon.reserve(_polygonASize + _polygonBSize - 2);
      for (size_t k = i2; k != i; k = (k + 1) % _polygonASize) {
        polygon.push_back((*polygonA)[k]);
      }
      for (size_t k = j2; k != j; k = (k + 1) % _polygonBSize) {
        polygon.push_back((*polygonB)[k]);
      }
      *polygonA = std::move(polygon);
      triangles.erase(polygonB);
      _polygonASize = polygonA->size();
      verticeIdx = 0;
    }
  }
  return triangles;
}

template <typename PolyItB, typename PolyItA>
std::pair<bool, size_t> PolygonPartitioner::findDiagonal(const std::vector<Triangle>& triangles, PolyItB& polygonB,
                                                         const PolyItA& polygonA, size_t i) const {
  const auto& pA1 = (*polygonA)[i];
  const auto& pA2 = (*polygonA)[(i + 1) % _polygonASize];
  size_t j;

  for (polygonB = std::next(polygonA); polygonB != triangles.end(); ++polygonB) {
    _polygonBSize = polygonB->size();

    for (j = 0; j < _polygonBSize; ++j) {
      const auto& pB1 = (*polygonB)[j];
      const auto& pB2 = (*polygonB)[(j + 1) % _polygonBSize];

      if (pA2 == pB1 && pA1 == pB2) { // Diagonal found
        return { true, j };
      }
    }
  }
  return { false, j };
}

bool PolygonPartitioner::isPolygonConvex(const Polygon& polygon) const {
  size_t polygonSize = polygon.size();
  size_t i;

  for (i = 0; i < polygonSize; ++i) {
    size_t prev = (i + polygonSize - 1) % polygonSize;
    size_t next = (i + 1) % polygonSize;
    if (_triangulator.isAngleReflex(polygon[prev], polygon[i], polygon[next])) {
      break;
    }
  }

  return (i == polygonSize);
}

} /* namespace Detail */
} /* namespace HitboxBuilder */
