#pragma once
#include <string>
#include <cstdint>

#include "Transform.h"
#include "RandomRotateType.h"

struct EffectSpawnParams {
	std::string name;    //名前
	EulerTransform transform; //transform
	RandomRotateType randomRotateType = RandomRotateType::None; //ランダム回転の軸
	uint32_t count;      //生成するパーティクルの数
	float lifeTime;      //パーティクルの寿命
	float minSpeed;      //パーティクルの最小初速
	float maxSpeed;      //パーティクルの最大初速
};