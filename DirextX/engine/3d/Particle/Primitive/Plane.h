#pragma once
#include "Primitive.h"
class Plane : public Primitive
{
public:
	Plane();
	void Initialize();

	void Update() override;

	void CreateVertex();

	void CreateIndex();
};

