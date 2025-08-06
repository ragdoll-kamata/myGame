#pragma once
#include "EmissionShape.h"
#include "Sphere.h"
class SphereShape :
    public EmissionShape {
public:
	/// <summary>
	/// 球の範囲内でランダムに位置を生成する
	/// </summary>
	/// <param name="sphere">範囲</param>
	SphereShape(Sphere sphere);
	Vector3 EmitPosition(std::mt19937& random) override;
private:
	Sphere sphere_;
};

