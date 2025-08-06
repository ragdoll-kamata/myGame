#pragma once
#include "Vector3.h"
#include <random>
class EmissionShape {
public:
	virtual ~EmissionShape() = default;

	virtual Vector3 EmitPosition(std::mt19937& random) = 0;

protected:
};

