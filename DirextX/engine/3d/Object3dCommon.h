#pragma once
#include "DirectXCommon.h"

class Object3dCommon
{
public:
	void Initialize(DirectXCommon* directXCommon);

	void PreDraw();

	void IfSkinningDraw(bool is);

	DirectXCommon* GetDirectXCommon()const { return directXCommon_; }

	static Object3dCommon* GetInstance();
	void Finalize();
private:
	void CreateRootSignature();


	void CreateGraphicsPipeline();
	void CreateSkinningGraphicsPipeline();

private:
	DirectXCommon* directXCommon_ = nullptr;

	bool isSkinning = false;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> skinningGraphicsPipelineState = nullptr;

private:
	static Object3dCommon* instance;

	Object3dCommon() = default;
	~Object3dCommon() = default;
	Object3dCommon(Object3dCommon&) = default;
	Object3dCommon& operator=(Object3dCommon&) = default;
};

