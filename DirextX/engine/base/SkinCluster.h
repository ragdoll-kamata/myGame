#pragma once
#include <vector>
#include <span>

#include <DirectXCommon.h>
#include <SrvManager.h>
#include <Skeleton.h>

#include <Matrix4x4.h>


const uint32_t kNumMaxInfluence = 4;  
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};
struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

class SkinCluster {
public:
	void Initialize(const std::string& modelFilePath, Skeleton* skeleton);

	void Update();

	D3D12_VERTEX_BUFFER_VIEW GetInfluenceBufferView(uint32_t index) const {
		return influenceBufferViews[index];
	}
	D3D12_GPU_DESCRIPTOR_HANDLE GetPaletteSrvHandle() const {
		return paletteSrvHandle.second;
	}
	static void SetDirectXCommon(DirectXCommon* directXCommon) {
		directXCommon_ = directXCommon;
	}
	static void SetSrvManager(SrvManager* srvManager) {
		srvManager_ = srvManager;
	}

private:
	static DirectXCommon* directXCommon_;
	static SrvManager* srvManager_;

	Skeleton* skeleton_ = nullptr;

	std::vector<Matrix4x4> inverseBindPoseMatrices;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> influenceResources;
	std::vector<D3D12_VERTEX_BUFFER_VIEW> influenceBufferViews;
	std::vector<std::span<VertexInfluence>> mappedInfluences;

	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette_;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};

