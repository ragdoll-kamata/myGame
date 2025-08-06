#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <optional>

#include <Transform.h>
#include <Matrix4x4.h>
#include <Node.h>

class Animation;

class Skeleton {
public:
	struct Joint {
		QuaternionTransform transform;
		Matrix4x4 localMatrix;
		Matrix4x4 skeletonSpaceMatrix;
		std::string name;
		std::vector<int32_t> children;
		int32_t index;
		std::optional<int32_t> parent;
	};
	void Initialize(const Node& node);

	void Update();

	void NextAnimation(Animation* animation, float time);

	void LineJoint(int32_t i);

	void SetAnimation(Animation* animation) {
		animation_ = animation;
	}
	Joint& GetJoint(int32_t index) {
		return joints[index];
	}
	int32_t GetJointIndex(const std::string& name) const {
		auto it = jointMap.find(name);
		if (it != jointMap.end()) {
			return it->second;
		}
		return -1; // Joint not found
	}
	int32_t GetJointsCount() const {
		return static_cast<int32_t>(joints.size());
	}

private:
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent = std::nullopt);
	void UpdateJoint(int32_t i);
	void ApplyAnimation();
private:
	Animation* animation_ = nullptr;
	Animation* nextAnimation_ = nullptr;
	float transitionTime_ = 0.0f;
	float transitionDuration_ = 0.0f;

	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;

};

