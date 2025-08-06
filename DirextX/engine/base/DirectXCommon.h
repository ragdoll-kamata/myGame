#pragma once
#include "WinApp.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <dxcapi.h>
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/d3dx12.h"
#include <chrono>
#include <vector>
class DirectXCommon
{
public:
	void Initialize(WinApp* winApp);

	void PreDraw();

	void PostDraw();

	void Finalize() const;

	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	size_t GetSwapChainResourcesNum() const { return swapChainDesc.BufferCount; }

	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextResource(UINT width, UINT height);
	
	void UploadTextData(ID3D12Resource* pResource, std::vector<uint8_t> data, unsigned int width, unsigned int height);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextTexture2DArray(UINT width, UINT height, UINT layer);

	void AAAAAAAAAAAAAA();

	void UploodTextTexture(ID3D12Resource* pResource,std::vector<uint8_t> data);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
private:
	void InitializeFixFPS();

	void UpdateFixFPS();

	//
	void CreateDevice();

	void CreateCommand();

	void CreateSwapChain();

	void CreateDepthStencilTextureResource();

	void CreateAllDescriptorHeap();

	void RTVInitialize();

	void DepthStencilInitialize();

	void CreateFence();

	void ViewportInitialize();

	void ScissorRectInitialize();

	void CreateDxcCompiler();

	


	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUdescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUdescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

private:
	//
	WinApp* winApp_ = nullptr;

	std::chrono::steady_clock::time_point reference_;

	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	HANDLE fenceEvent;

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	Microsoft::WRL::ComPtr <IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	UINT64 fenceValue = 0;
};

