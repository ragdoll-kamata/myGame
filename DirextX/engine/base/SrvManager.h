#pragma once
#include "DirectXCommon.h"
class SrvManager
{
public:
	void Initialize(DirectXCommon* directXCommon);

	void PreDraw();

	uint32_t Allocate();

	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResourece, DXGI_FORMAT Format, UINT MipLevels);

	void CreateSRVforTexture2DArray(uint32_t srvIndex, ID3D12Resource* pResourece, DXGI_FORMAT Format, UINT MipLevels, UINT arraySize);

	void CreateSRVforStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResourece, UINT numElements, UINT structureBvteStride);

	bool IsMaxSrvNotOver() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUdescriptorHandle(uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUdescriptorHandle(uint32_t index);

	ID3D12DescriptorHeap* GetDescriptorHeap() const { return descriptorHeap.Get(); }

	static const uint32_t kMaxSRVCount = 512;
private:
	DirectXCommon* directXCommon_ = nullptr;
	uint32_t descriptorSize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	uint32_t useIndex = 0;
};

