#pragma once
#include "EmissionShape.h"
class LineShape : public EmissionShape {
public:
	void Initialize(Vector3 start, Vector3 end);
	Vector3 EmitPosition(std::mt19937& random) override;
private:
	Vector3 start_;
	Vector3 end_;
};

