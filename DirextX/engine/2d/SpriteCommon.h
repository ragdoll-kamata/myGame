#pragma once
#include "DirectXCommon.h"

class SpriteCommon
{
public:
	void Initialize(DirectXCommon* directXCommon);

	void PreDraw();

	DirectXCommon* GetDirectXCommon()const { return directXCommon_; }

	static SpriteCommon* GetInstance();
	void Finalize();
private:
	void CreateRootSignature();

	void CreateGraphicsPipeline();

private:
	DirectXCommon* directXCommon_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

private:
	static SpriteCommon* instance;

	SpriteCommon() = default;
	~SpriteCommon() = default;
	SpriteCommon(SpriteCommon&) = default;
	SpriteCommon& operator=(SpriteCommon&) = default;
};

