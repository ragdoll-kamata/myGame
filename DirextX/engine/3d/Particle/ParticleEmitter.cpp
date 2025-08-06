#include "ParticleEmitter.h"
#include "DeltaTime.h"
#include "ParticleManager.h"
#include "EffectSpawnParams.h"

#include "MathUtility.h"
using namespace MathUtility;

void ParticleEmitter::Initialize(const Emitter& emitter, std::unique_ptr<EmissionShape> emissionShape) {
	emitter_ = emitter;
	emitter_.frequencyTime = 0.0f;
	emissionShape_ = std::move(emissionShape);
}

void ParticleEmitter::Update() {
	if (emitter_.isTimer) {
		emitter_.frequencyTime += DeltaTime::GetInstance()->GetDeltaTime();

		if (emitter_.frequency <= emitter_.frequencyTime) {
			Emit();
			emitter_.frequencyTime -= emitter_.frequency;
		}
	}
}

void ParticleEmitter::Emit() {
	EffectSpawnParams effectSpawnParams;
	effectSpawnParams.name = emitter_.name;
	effectSpawnParams.transform.translate = emitter_.center;
	effectSpawnParams.transform.scale = emitter_.scale;
	effectSpawnParams.randomRotateType = emitter_.randomRotateType;
	effectSpawnParams.count = emitter_.count;
	effectSpawnParams.lifeTime = emitter_.lifeTime;
	effectSpawnParams.minSpeed = emitter_.minSpeed;
	effectSpawnParams.maxSpeed = emitter_.maxSpeed;
	// 生成
	ParticleManager::GetInstance()->Emit(effectSpawnParams, emissionShape_.get());
}


