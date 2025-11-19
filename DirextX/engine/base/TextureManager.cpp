#include "TextureManager.h"
#include "DirectXCommon.h"
#include "StringUtility.h"
#include "SrvManager.h"
TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop = 1;
void TextureManager::Initialize(DirectXCommon* directXCommon, SrvManager* srvManager)
{
	directXCommon_ = directXCommon;
	textureDatas.reserve(SrvManager::kMaxSRVCount);
	srvManager_ = srvManager;
	directoryPath_ = "Resource/Texture";
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	if (textureDatas.contains(filePath)) {
		return;
	}
	assert(srvManager_->IsMaxSrvNotOver());
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(directoryPath_ + "/" + filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));
	TextureData a;
	textureDatas.insert(std::make_pair(filePath, a));
	TextureData& textureData = textureDatas[filePath];
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = directXCommon_->CreateTextureResource(textureData.metadata);
	directXCommon_->UploadTextureData(textureData.resource.Get(), mipImages);


	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUdescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUdescriptorHandle(textureData.srvIndex);

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	LoadTexture(filePath);
	if (textureDatas.contains(filePath)) {
		return textureDatas[filePath].srvIndex;
	}

	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(const std::string& filePath)
{
	assert(textureDatas.contains(filePath));
	
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& filePath)
{
	TextureData& a = textureDatas[filePath];
	return a.metadata;
}

TextureManager* TextureManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new TextureManager;
    }
    return instance;
}

void TextureManager::Finalize()
{
    delete instance;
    instance = nullptr;
}
