#pragma once
#include <string>
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <unordered_map>
class DirectXCommon;
class SrvManager;

class TextureManager
{
public:
	void Initialize(DirectXCommon* directXCommon, SrvManager* srvManager);

	void SetDirectoryPath(std::string directoryPath) {
		directoryPath_ = directoryPath;
	}

	void LoadTexture(const std::string& filePath);

	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& filePath);

	DirectX::TexMetadata& GetMetadata(const std::string& filePath);

	static TextureManager* GetInstance();
	static uint32_t kSRVIndexTop;
	void Finalize();
private:
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};
	std::unordered_map<std::string, TextureData> textureDatas;

	std::string directoryPath_;




private:
	static TextureManager* instance;

	DirectXCommon* directXCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;
};

