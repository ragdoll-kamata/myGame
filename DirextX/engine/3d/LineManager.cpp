#include "LineManager.h"
#include <Logger.h>

#include<MathUtility.h>

#include <CameraManager.h>

using namespace MathUtility;


LineManager* LineManager::instance = nullptr;

void LineManager::Initialize(DirectXCommon* directXCommon, SrvManager* srvManager) {
	this->directXCommon_ = directXCommon;
	srvManager_ = srvManager;
	CreateRootSignature();
	CreateGraphicsPipeline();

	CreateLineData();
}

void LineManager::Update() {
	lineCount = 0;
	Matrix4x4 viewProjectionMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
	Vector3 pos;
	for (auto& line : lines) {
		pos.x = line.startPos.x * line.startScale.x;
		pos.y = line.startPos.y * line.startScale.y;
		pos.z = line.startPos.z * line.startScale.z;
		linesData[lineCount].startWVP = MathUtility::MakeAffineMatrix({1.0f, 1.0f, 1.0f}, Vector3{}, pos) * viewProjectionMatrix;
		pos.x = line.endPos.x * line.endScale.x;
		pos.y = line.endPos.y * line.endScale.y;
		pos.z = line.endPos.z * line.endScale.z;
		linesData[lineCount].endWVP = MathUtility::MakeAffineMatrix({1.0f, 1.0f, 1.0f}, Vector3{}, pos) * viewProjectionMatrix;
		linesData[lineCount].color = line.color;
		lineCount++;
		if (lineCount >= kMaxLines) {
			Logger::Log("Line count exceeded maximum limit.");
			break;
		}
	}
}

void LineManager::Draw() {
	if (lineCount > 0) {
		ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
		commandList->SetGraphicsRootSignature(rootSignature.Get());
		commandList->SetPipelineState(graphicsPipelineState.Get());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		commandList->SetGraphicsRootDescriptorTable(0, srvManager_->GetGPUdescriptorHandle(linesDataSrvIndex));
		commandList->DrawInstanced(lineCount * 2, 1, 0, 0);
	}
}

void LineManager::CreateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);


	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//
	hr = directXCommon_->GetDevice()->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void LineManager::CreateGraphicsPipeline() {


	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
		directXCommon_->CompileShader(L"Resource/Shaders/Line.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
		directXCommon_->CompileShader(L"Resource/Shaders/Line.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.VS = {vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.PS = {pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//
	HRESULT hr = directXCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void LineManager::CreateLineData() {
	linesResource = directXCommon_->CreateBufferResource(sizeof(LineData) * kMaxLines);
	linesResource->Map(0, nullptr, reinterpret_cast<void**>(&linesData));
	linesDataSrvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforStructureBuffer(linesDataSrvIndex, linesResource.Get(), kMaxLines, sizeof(LineData));
}

LineManager* LineManager::GetInstance() {
	if (instance == nullptr) {
		instance = new LineManager();
	}
	return instance;
}

void LineManager::Finalize() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}