#include "Animation.h"
#include<cassert>

#include "DeltaTime.h"
#include "MathUtility.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Animation::Initialize(const std::string& directoryPath, const std::string& filename) {
	LoadAnimation(directoryPath, filename);
	animationTime_ = 0.0f;
}
void Animation::Update() {
	animationTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	animationTime_ = std::fmod(animationTime_, duration_);
}

void Animation::LoadAnimation(const std::string& directoryPath, const std::string& filename) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath, 0);
	assert(scene->HasAnimations());
	assert(scene->mNumAnimations != 0);

	aiAnimation* aiAnimation = scene->mAnimations[0];
	duration_ = static_cast<float>(aiAnimation->mDuration / aiAnimation->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < aiAnimation->mNumChannels; ++channelIndex) {
		aiNodeAnim* aiNodeAnimation = aiAnimation->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = nodeAnimation_[aiNodeAnimation->mNodeName.C_Str()];
		for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = aiNodeAnimation->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / aiAnimation->mTicksPerSecond);
			keyframe.value = {-keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z};
			nodeAnimation.translate.keyframes.push_back(keyframe);

		}
		for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = aiNodeAnimation->mRotationKeys[keyIndex];
			KeyframeVector4 keyframe;
			keyframe.time = float(keyAssimp.mTime / aiAnimation->mTicksPerSecond);
			keyframe.value = {keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w};
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < aiNodeAnimation->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = aiNodeAnimation->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / aiAnimation->mTicksPerSecond);
			keyframe.value = {keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z};
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}

}

Vector3 Animation::TranslateCalculateValue(std::string nodeName) {
	assert(nodeAnimation_.contains(nodeName) && "Node animation not found");
	animationTime_ = std::fmod(animationTime_, duration_);
	auto& nodeAnim = nodeAnimation_[nodeName];
	auto& keyframes = nodeAnim.translate.keyframes;
	if (keyframes.empty()) {
		return Vector3();
	}
	if(keyframes.size() == 1 || animationTime_ <= keyframes[0].time) {
		return keyframes[0].value;
	}
	
	for(size_t i = 0; i < keyframes.size() - 1; ++i) {
		if (animationTime_ >= keyframes[i].time && animationTime_ <= keyframes[i + 1].time) {
			float t = (animationTime_ - keyframes[i].time) / (keyframes[i + 1].time - keyframes[i].time);
			return MathUtility::Lerp(keyframes[i].value, keyframes[i + 1].value, t);
		}
	}
	return Vector3();
}

Vector4 Animation::RotateCalculateValue(std::string nodeName) {
	assert(nodeAnimation_.contains(nodeName) && "Node animation not found");
	animationTime_ = std::fmod(animationTime_, duration_);
	auto& nodeAnim = nodeAnimation_[nodeName];
	auto& keyframes = nodeAnim.rotate.keyframes;
	if (keyframes.empty()) {
		return Vector4();
	}
	if (keyframes.size() == 1 || animationTime_ <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t i = 0; i < keyframes.size() - 1; ++i) {
		if (animationTime_ >= keyframes[i].time && animationTime_ <= keyframes[i + 1].time) {
			float t = (animationTime_ - keyframes[i].time) / (keyframes[i + 1].time - keyframes[i].time);
			return MathUtility::SLerp(keyframes[i].value, keyframes[i + 1].value, t);
		}
	}
	return Vector4();
}

Vector3 Animation::ScaleCalculateValue(std::string nodeName) {
	assert(nodeAnimation_.contains(nodeName) && "Node animation not found");
	animationTime_ = std::fmod(animationTime_, duration_);
	auto& nodeAnim = nodeAnimation_[nodeName];
	auto& keyframes = nodeAnim.scale.keyframes;
	if (keyframes.empty()) {
		return Vector3();
	}
	if (keyframes.size() == 1 || animationTime_ <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t i = 0; i < keyframes.size() - 1; ++i) {
		if (animationTime_ >= keyframes[i].time && animationTime_ <= keyframes[i + 1].time) {
			float t = (animationTime_ - keyframes[i].time) / (keyframes[i + 1].time - keyframes[i].time);
			return MathUtility::Lerp(keyframes[i].value, keyframes[i + 1].value, t);
		}
	}
	return Vector3();
}
