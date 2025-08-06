#include "SphereShape.h"
#include "MathUtility.h"
#include <cmath>
using namespace MathUtility;
SphereShape::SphereShape(Sphere sphere) {
	sphere_ = sphere;
}

Vector3 SphereShape::EmitPosition(std::mt19937& random) {

    float u = RandomFloat(0.0f, 1.0f, random);
    float theta = RandomFloat(0.0f, MathUtility::kPi * 2.0f, random);
    float phi = acos(1 - 2 * RandomFloat(0.0f, 1.0f, random)); // 一様分布のための変換

    float r = sphere_.radius * cbrt(u); // 体積均等のために立方根を取る

    Vector3 result;
    result.x = r * sin(phi) * cos(theta);
    result.y = r * sin(phi) * sin(theta);
    result.z = r * cos(phi);

    return result + sphere_.center;
}
