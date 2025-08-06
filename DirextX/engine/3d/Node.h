#pragma once
#include <string>
#include <vector>

#include <Transform.h>
#include "Matrix4x4.h"
struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};