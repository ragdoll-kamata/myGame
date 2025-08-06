#pragma once
#include "Field.h"
class WindField : public Field
{
public:
	WindField();
	~WindField() = default;

	void Update() override;

	void Hit(Particle& particl) override;

	AABB GetAABB() override;
	void SetAABB(AABB& aabb) override;

private:
	AABB range_;
	Vector3 pos;
	Vector3 direction;
	float pow;
};
