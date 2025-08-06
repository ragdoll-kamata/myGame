#pragma once
#include "EmissionShape.h"
#include "AABB.h"
class AABBShape : public EmissionShape {
public:
	/// <summary>
	/// AABBの範囲内でランダムに位置を生成する
	/// </summary>
	/// <param name="aabb">範囲</param>
	AABBShape(AABB aabb);
	Vector3 EmitPosition(std::mt19937& random) override;
private:
	AABB aabb_;

};

