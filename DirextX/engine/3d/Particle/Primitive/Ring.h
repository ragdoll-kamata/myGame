#pragma once
#include "Primitive.h"
class Ring : public Primitive
{
public:
	Ring();
	void Initialize(uint32_t division, float outerRadius, float innerRadius);
	void Update() override;
	void CreateVertex(int division);
	void CreateIndex(int division);

private:
	uint32_t division_;
};

