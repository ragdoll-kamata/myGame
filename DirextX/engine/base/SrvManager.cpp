#include "SrvManager.h"
#include <cassert>

void SrvManager::Initialize(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
	descriptorSize = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorHeap = directXCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
}

void SrvManager::PreDraw()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	directXCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

uint32_t SrvManager::Allocate()
{
	assert(useIndex < kMaxSRVCount);
	int index = useIndex;
	useIndex++;
	return index;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResourece, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = MipLevels;

	directXCommon_->GetDevice()->CreateShaderResourceView(pResourece, &srvDesc, GetCPUdescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforTexture2DArray(uint32_t srvIndex, ID3D12Resource* pResourece, DXGI_FORMAT Format, UINT MipLevels, UINT arraySize) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

	srvDesc.Texture2DArray.MipLevels = MipLevels;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = arraySize;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;

	directXCommon_->GetDevice()->CreateShaderResourceView(pResourece, &srvDesc, GetCPUdescriptorHandle(srvIndex));
}


void SrvManager::CreateSRVforStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResourece, UINT numElements, UINT structureBvteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = numElements;
	instancingSrvDesc.Buffer.StructureByteStride = structureBvteStride;

	directXCommon_->GetDevice()->CreateShaderResourceView(pResourece, &instancingSrvDesc, GetCPUdescriptorHandle(srvIndex));
}

bool SrvManager::IsMaxSrvNotOver() const
{
	if (useIndex < kMaxSRVCount) {
		return true;
	}
	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUdescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUdescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

