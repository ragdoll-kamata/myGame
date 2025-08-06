#include "LineShape.h"

#include "MathUtility.h"

using namespace MathUtility;

void LineShape::Initialize(Vector3 start, Vector3 end) {
	start_ = start;
	end_ = end;
}

Vector3 LineShape::EmitPosition(std::mt19937& random) {
	float t = RandomFloat(0.0f, 1.0f, random);

	return Lerp(start_, end_, t);
}
