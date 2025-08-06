#pragma once
#include <string>
#include <vector>
#include <map>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeVector4 = Keyframe<Vector4>;

template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};


class Animation {
public:
	struct NodeAnimation {
		AnimationCurve<Vector3> translate;
		AnimationCurve<Vector4> rotate;
		AnimationCurve<Vector3> scale;
		
	};

	void Initialize(const std::string& directoryPath, const std::string& filename);

	void Update();

	NodeAnimation* GetNodeAnimation(const std::string& nodeName) {
		if (nodeAnimation_.contains(nodeName)) {
			return &nodeAnimation_[nodeName];
		}
		return nullptr;
	}
	Vector3 TranslateCalculateValue(std::string nodeName);
	Vector4 RotateCalculateValue(std::string nodeName);
	Vector3 ScaleCalculateValue(std::string nodeName);

private:
	void LoadAnimation(const std::string& directoryPath, const std::string& filename);
private:
	float duration_;
	std::map<std::string, NodeAnimation> nodeAnimation_;

	float animationTime_ = 0.0f;
};

