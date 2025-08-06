#pragma once
#include "Primitive.h"
class Cylinder : public Primitive
{
public:
	void Initialize (uint32_t division, float radius, float height);
	void Update() override;
	void CreateVertex (int division);
	void CreateIndex (int division);
private:
	uint32_t division_;
};

