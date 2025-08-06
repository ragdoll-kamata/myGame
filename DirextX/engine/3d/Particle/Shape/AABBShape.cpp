#include "AABBShape.h"
#include "MathUtility.h"

using namespace MathUtility;

AABBShape::AABBShape(AABB aabb) {
	aabb_ = aabb;
}

Vector3 AABBShape::EmitPosition(std::mt19937& random) {
	Vector3 min = aabb_.min;
	Vector3 max = aabb_.max;
	float x = RandomFloat(min.x, max.x, random);
	float y = RandomFloat(min.y, max.y, random);
	float z = RandomFloat(min.z, max.z, random);
	return Vector3(x, y, z);
}
