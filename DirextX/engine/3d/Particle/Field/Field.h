#pragma once
#include <variant>
#include "Sphere.h"
#include "AABB.h"
#include "Particle.h"
class Field
{
public:
	Field() = default;
	~Field() = default;

	virtual void Update() = 0;

	virtual void Hit(Particle& particl) = 0;

	virtual Sphere GetSphere();
	virtual void SetSphere(Sphere& sphere);

	virtual AABB GetAABB();
	virtual void SetAABB(AABB& aabb);

protected:
	
};

