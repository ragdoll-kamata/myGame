#pragma once

#include "FontAtlasManager.h"
#include "FontLoader.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "IndexBuffer.h"

class TextCommon {
public:
	void Initialize(DirectXCommon* directXCommon, SrvManager* srvManager, std::string font, std::string directory = "resource/font");
	void PreDraw();

	void PostDraw();

	FontLoader* GetFontLoader() {
		return fontLoader.get();
	}
	FontAtlasManager* GetFontAtlasManager() {
		return fontAtlasManager.get();
	}
	DirectXCommon* GetDirectXCommon() {
		return directXCommon_;
	}
	SrvManager* GetSrvManager() {
		return srvManager_;
	}

	static TextCommon* GetInstance();
	void Finalize();
private:
	void CreateRootSignature();
	void CreateGraphicsPipeline();

	void CreateIndex();
private:
	DirectXCommon* directXCommon_;
	SrvManager* srvManager_;
	std::unique_ptr<FontLoader> fontLoader;
	std::unique_ptr<FontAtlasManager> fontAtlasManager;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	IndexBuffer indexBuffer;
private:
	static TextCommon* instance;

	TextCommon() = default;
	~TextCommon() = default;
	TextCommon(TextCommon&) = default;
	TextCommon& operator=(TextCommon&) = default;
};

