#pragma once
#include "EmissionShape.h"

#include "Vector3.h"

class PointShape :
    public EmissionShape {
public:
	PointShape(Vector3 point);
	~PointShape() override;
	Vector3 EmitPosition(std::mt19937& random) override;
private:
	Vector3 point_;
};

