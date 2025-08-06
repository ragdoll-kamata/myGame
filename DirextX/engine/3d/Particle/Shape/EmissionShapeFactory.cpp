#include "EmissionShapeFactory.h"

std::unique_ptr<EmissionShape> EmissionShapeFactory::CreatePointShape(const Vector3& point) {
	return std::make_unique<PointShape>(point);
}

std::unique_ptr<EmissionShape> EmissionShapeFactory::CreateSphereShape(const Sphere& sphere) {
	return std::make_unique<SphereShape>(sphere);
}

std::unique_ptr<EmissionShape> EmissionShapeFactory::CreateAABBShape(const AABB& aabb) {
	return std::make_unique<AABBShape>(aabb);
}
