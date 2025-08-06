#include "SkinCluster.h"
#include "ModelManager.h"
#include "Model.h"
#include "MathUtility.h"

using namespace MathUtility;

DirectXCommon* SkinCluster::directXCommon_ = nullptr;
SrvManager* SkinCluster::srvManager_ = nullptr;


void SkinCluster::Initialize(const std::string& modelFilePath, Skeleton* skeleton) {
	Model* model = ModelManager::GetInstance()->FindModel(modelFilePath);
	skeleton_ = skeleton;
	if (model != nullptr && skeleton_ != nullptr) {
		uint32_t jointCount = skeleton_->GetJointsCount();
		paletteResource = directXCommon_->CreateBufferResource(sizeof(WellForGPU) * jointCount);
		WellForGPU* mappedPalette = nullptr;
		paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
		mappedPalette_ = {mappedPalette, jointCount};
		uint32_t index = srvManager_->Allocate();
		paletteSrvHandle.first = srvManager_->GetCPUdescriptorHandle(index);
		paletteSrvHandle.second = srvManager_->GetGPUdescriptorHandle(index);

		srvManager_->CreateSRVforStructureBuffer(index, paletteResource.Get(), jointCount, sizeof(WellForGPU));

		inverseBindPoseMatrices.resize(jointCount);
		for (Matrix4x4& in : inverseBindPoseMatrices) {
			in = MakeIdentity4x4();
		}

		influenceResources.resize(model->GetModelData()->meshDatas.size());
		mappedInfluences.resize(model->GetModelData()->meshDatas.size());
		influenceBufferViews.resize(model->GetModelData()->meshDatas.size());
		uint32_t meshIndex = 0;
		for (const auto& meshData : model->GetModelData()->meshDatas) {
			uint32_t vertexSize = static_cast<uint32_t>(meshData.vertices.size());

			influenceResources[meshIndex] = directXCommon_->CreateBufferResource(sizeof(VertexInfluence) * vertexSize);
			VertexInfluence* mappedInfluence = nullptr;
			influenceResources[meshIndex]->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
			std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * vertexSize);
			mappedInfluences[meshIndex] = {mappedInfluence, vertexSize};

			influenceBufferViews[meshIndex].BufferLocation = influenceResources[meshIndex]->GetGPUVirtualAddress();
			influenceBufferViews[meshIndex].SizeInBytes = UINT(sizeof(VertexInfluence) * vertexSize);
			influenceBufferViews[meshIndex].StrideInBytes = sizeof(VertexInfluence);

			for (const auto& jointWeight : meshData.skinClusterData) {
				int32_t jointIndex = skeleton_->GetJointIndex(jointWeight.first);
				if (jointIndex < 0) {
					continue; //　0よりしたならそんな名前のジョイントはない
				}
				inverseBindPoseMatrices[jointIndex] = jointWeight.second.inverseBindPoseMatrix;
				for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
					auto& currentInfluence = mappedInfluences[meshIndex][vertexWeight.vertexIndex];
					for (uint32_t index = 0; index < kNumMaxInfluence; index++) {
						if (currentInfluence.weights[index] == 0.0f) {
							currentInfluence.weights[index] = vertexWeight.weight;
							currentInfluence.jointIndices[index] = jointIndex;
							break; // 空いているインデックスに追加
						}
					}

				}
			}

			meshIndex++;
		}

	}
}

void SkinCluster::Update() {
	for(int32_t jointIndex = 0; jointIndex < skeleton_->GetJointsCount(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices.size());
		mappedPalette_[jointIndex].skeletonSpaceMatrix =
			inverseBindPoseMatrices[jointIndex] * skeleton_->GetJoint(jointIndex).skeletonSpaceMatrix;
		mappedPalette_[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
	}
}