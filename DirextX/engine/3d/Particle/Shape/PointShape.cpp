#include "PointShape.h"

PointShape::PointShape(Vector3 point) : point_(point) {
}

PointShape::~PointShape() {
}

Vector3 PointShape::EmitPosition(std::mt19937& random) {
	return point_;
}
