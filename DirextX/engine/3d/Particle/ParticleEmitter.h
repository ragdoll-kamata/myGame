#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <random>

#include "Vector3.h"
#include "Transform.h"
#include "EmissionShape.h"
#include "EmissionShapeFactory.h"
#include "RandomRotateType.h"

class ParticleEmitter
{
public:

	struct Emitter {
		std::string name;    //名前
		Vector3 center;      //中心位置
		Vector3 scale = {1.0f, 1.0f, 1.0f};      //スケール
		RandomRotateType randomRotateType = RandomRotateType::None; //ランダム回転の軸
		uint32_t count;      //生成するパーティクルの数
		float lifeTime;      //パーティクルの寿命
		float minSpeed;      //パーティクルの最小初速
		float maxSpeed;      //パーティクルの最大初速
		float frequency;     //生成する間隔
		float frequencyTime; //生成する間隔のカウント
		bool isTimer = true; //タイマーの有無
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="emitter">エミッター</param>
	/// <param name="emissionShape">発生範囲</param>
	void Initialize(const Emitter& emitter, std::unique_ptr<EmissionShape> emissionShape);

	/// <summary>
	/// 発生範囲の再設定
	/// </summary>
	/// <param name="emissionShape">発生範囲</param>
	void SetEmissionShape(std::unique_ptr<EmissionShape> emissionShape) {
		emissionShape_ = std::move(emissionShape);
	}

	void Update();

	void Trigger() {
		if (emitter_.isTimer) {
			return;
		}
		Emit();
	}

private:
	void Emit();


private:
	Emitter emitter_;
	std::unique_ptr<EmissionShape> emissionShape_;
	
	std::mt19937 random_; // 乱数生成器
};

