#pragma once
#include <memory>

#include "PointShape.h"
#include "SphereShape.h"
#include "AABBShape.h"
namespace EmissionShapeFactory {

	std::unique_ptr<EmissionShape> CreatePointShape(const Vector3& point);
	std::unique_ptr<EmissionShape> CreateSphereShape(const Sphere& sphere);
	std::unique_ptr<EmissionShape> CreateAABBShape(const AABB& aabb);
};

