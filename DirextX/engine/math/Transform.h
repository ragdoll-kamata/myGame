#pragma once
#include "Vector3.h"
#include "Vector4.h"

struct EulerTransform final {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct QuaternionTransform final {
	Vector3 scale;
	Vector4 rotate;
	Vector3 translate;
};