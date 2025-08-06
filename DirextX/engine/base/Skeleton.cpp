#include "Skeleton.h"
#include "MathUtility.h"
#include "LineManager.h"
#include "Animation.h"
#include "DeltaTime.h"
using namespace MathUtility;
void Skeleton::Initialize(const Node& node) {
	root = CreateJoint(node);
	for (const Joint& joint : joints) {
		jointMap.emplace(joint.name, joint.index);
	}
}

void Skeleton::Update() {
	if (transitionDuration_ > 0.0f) {
		transitionTime_ += DeltaTime::GetInstance()->GetDeltaTime();
		if (transitionDuration_ <= transitionTime_) {
			animation_ = nextAnimation_;
			nextAnimation_ = nullptr;
			transitionDuration_ = 0.0f;
			transitionTime_ = 0.0f;
		}
	}
	if (animation_) {
		ApplyAnimation();
	}
	for (Joint& joint : joints) {
		joint.localMatrix = MathUtility::MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * joints[*joint.parent].skeletonSpaceMatrix;
		} else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
	//LineJoint(root);
}

void Skeleton::NextAnimation(Animation* animation, float time) {
	nextAnimation_ = animation;
	transitionDuration_ = time;
}

void Skeleton::LineJoint(int32_t i) {
	Joint& joint = joints[i];
	if (joint.parent) {
		Vector3 pos = {joints[*joint.parent].skeletonSpaceMatrix.m[3][0], joints[*joint.parent].skeletonSpaceMatrix.m[3][1], joints[*joint.parent].skeletonSpaceMatrix.m[3][2]};
		Vector3 pos2 = {joint.skeletonSpaceMatrix.m[3][0], joint.skeletonSpaceMatrix.m[3][1], joint.skeletonSpaceMatrix.m[3][2]};
		LineManager::GetInstance()->AddLine(pos, pos2);
	}
	for (const auto& child : joint.children) {
		LineJoint(child);
	}
}

int32_t Skeleton::CreateJoint(const Node& node, const std::optional<int32_t>& parent) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MathUtility::MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = static_cast<int32_t>(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const auto& child : node.children) {
		int32_t i = CreateJoint(child, joint.index);
		joints[joint.index].children.push_back(i);
	}
	return joint.index;
}

void Skeleton::UpdateJoint(int32_t i) {
	Joint& joint = joints[i];
	joint.localMatrix = MathUtility::MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
	if (joint.parent) {
		joint.skeletonSpaceMatrix = joint.localMatrix * joints[*joint.parent].skeletonSpaceMatrix;
	} else {
		joint.skeletonSpaceMatrix = joint.localMatrix;
	}
	for (const auto& child : joint.children) {
		UpdateJoint(child);
	}
}

void Skeleton::ApplyAnimation() {
	for (Joint& joint : joints) {
		if(nextAnimation_) {
			std::optional<QuaternionTransform> a;
			std::optional<QuaternionTransform> b;
			if (animation_->GetNodeAnimation(joint.name) != nullptr) {
				a = std::make_optional<QuaternionTransform>();
				a->translate = animation_->TranslateCalculateValue(joint.name);
				a->rotate = animation_->RotateCalculateValue(joint.name);
				a->scale = animation_->ScaleCalculateValue(joint.name);
			}
			if (nextAnimation_->GetNodeAnimation(joint.name) != nullptr) {
				b = std::make_optional<QuaternionTransform>();
				b->translate = nextAnimation_->TranslateCalculateValue(joint.name);
				b->rotate = nextAnimation_->RotateCalculateValue(joint.name);
				b->scale = nextAnimation_->ScaleCalculateValue(joint.name);
			}
			if (!a || !b) {
				if (a) {
					joint.transform.translate = a->translate;
					joint.transform.rotate = a->rotate;
					joint.transform.scale = a->scale;
				}
				if (b) {
					joint.transform.translate = b->translate;
					joint.transform.rotate = b->rotate;
					joint.transform.scale = b->scale;
				}
			} else {
				joint.transform.translate = MathUtility::Lerp(a->translate, b->translate, transitionTime_ / transitionDuration_);
				joint.transform.rotate = MathUtility::SLerp(a->rotate, b->rotate, transitionTime_ / transitionDuration_);
				joint.transform.scale = MathUtility::Lerp(a->scale, b->scale, transitionTime_ / transitionDuration_);
			}
			
		} else {
			if (animation_->GetNodeAnimation(joint.name) != nullptr) {
				joint.transform.translate = animation_->TranslateCalculateValue(joint.name);
				joint.transform.rotate = animation_->RotateCalculateValue(joint.name);
				joint.transform.scale = animation_->ScaleCalculateValue(joint.name);
			}
		}
	}

}
