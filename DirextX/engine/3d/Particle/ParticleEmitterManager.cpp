#include "ParticleEmitterManager.h"
#include <cassert>
void ParticleEmitterManager::Update() {
	for (const auto& [name, particleEmitter] : particleEmitters_) {
		particleEmitter->Update();
	}
}

void ParticleEmitterManager::CreateParticleEmitter(const std::string& name, const ParticleEmitter::Emitter& emitter, std::unique_ptr<EmissionShape> emissionShape) {
	assert(!particleEmitters_.contains(name));
	particleEmitters_.insert(std::make_pair(name, std::make_unique<ParticleEmitter>()));
	particleEmitters_[name]->Initialize(emitter, std::move(emissionShape));
}

void ParticleEmitterManager::TriggerParticleEmitter(const std::string& name) {
	assert(particleEmitters_.contains(name));
	particleEmitters_[name]->Trigger();
}

