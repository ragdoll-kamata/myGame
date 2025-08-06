#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include "ParticleEmitter.h"

class ParticleEmitterManager {
public:
	void Initialize();

	void Update();

	void CreateParticleEmitter(const std::string& name, const ParticleEmitter::Emitter& emitter, std::unique_ptr<EmissionShape> emissionShape);

	void TriggerParticleEmitter(const std::string& name);

	void RemoveParticleEmitter(const std::string& name) {
		particleEmitters_.erase(name);
	}
	void ClearParticleEmitter() {
		particleEmitters_.clear();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<ParticleEmitter>> particleEmitters_;
};

