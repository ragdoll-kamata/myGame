#include "WindField.h"

WindField::WindField()
{
}

void WindField::Update()
{
}

void WindField::Hit(Particle& particl)
{
}

AABB WindField::GetAABB()
{
	return range_;
}

void WindField::SetAABB(AABB& aabb)
{
	range_ = aabb;
}
