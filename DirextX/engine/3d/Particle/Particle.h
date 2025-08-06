#pragma once
#include "Transform.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct Particle final {
	EulerTransform transform; // ワールド行列
	Vector3 velocity;    // 速度
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);       // 色
	Vector2 uvScroll;    // uvスクロール
	Vector2 uvRotate;    // uv回転
	float lifeTime;      // 寿命
	float currentTime;   // 現在の時間
};