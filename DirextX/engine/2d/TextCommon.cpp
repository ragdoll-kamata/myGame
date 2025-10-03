#include "TextCommon.h"
#include "Logger.h"

TextCommon* TextCommon::instance = nullptr;
void TextCommon::Initialize(DirectXCommon* directXCommon, SrvManager* srvManager, std::string font, std::string directory) {
	directXCommon_ = directXCommon;
	srvManager_ = srvManager;
	fontLoader = std::make_unique<FontLoader>();
	fontLoader->Initialize(directory + "/" + font, 256);
	fontAtlasManager = std::make_unique<FontAtlasManager>();
	fontAtlasManager->Initialize(directXCommon_, fontLoader.get(), srvManager_, 2048, 2048);
	CreateRootSignature();
	CreateGraphicsPipeline();
	CreateIndex();


}

void TextCommon::PreDraw() {
	fontAtlasManager->PreDraw();
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(graphicsPipelineState.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetIndexBuffer(&indexBuffer.GetIndexBufferView());

	commandList->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUdescriptorHandle(fontAtlasManager->GetMainAtlasSrvIndex()));
	commandList->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUdescriptorHandle(fontAtlasManager->GetTempAtlasSrvIndex()));
}

void TextCommon::PostDraw() {
	fontAtlasManager->PostDraw();
}

TextCommon* TextCommon::GetInstance() {
	if (instance == nullptr) {
		instance = new TextCommon;
	}
	return instance;
}

void TextCommon::Finalize() {
	if (instance) {

		delete instance;
		instance = nullptr;
	}
}

void TextCommon::CreateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	D3D12_DESCRIPTOR_RANGE mainDescriptorRange[1] = {};
	mainDescriptorRange[0].BaseShaderRegister = 0;
	mainDescriptorRange[0].NumDescriptors = 1;
	mainDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	mainDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE tempDescriptorRange[1] = {};
	tempDescriptorRange[0].BaseShaderRegister = 1;
	tempDescriptorRange[0].NumDescriptors = 1;
	tempDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	tempDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = mainDescriptorRange;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(mainDescriptorRange);
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = tempDescriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(tempDescriptorRange);

	descriptionRootsignature.pParameters = rootParameters;
	descriptionRootsignature.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootsignature.pStaticSamplers = staticSamplers;
	descriptionRootsignature.NumStaticSamplers = _countof(staticSamplers);


	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
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

void TextCommon::CreateGraphicsPipeline() {
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDescs{};
	inputLayoutDescs.pInputElementDescs = inputElementDescs;
	inputLayoutDescs.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
		directXCommon_->CompileShader(L"resource/shaders/Text.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
		directXCommon_->CompileShader(L"resource/shaders/Text.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDescs;
	graphicsPipelineStateDesc.VS = {vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.PS = {pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize()};
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;



	//
	HRESULT hr = directXCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}
void TextCommon::CreateIndex() {
	indexBuffer.Initialize(directXCommon_, 6);

	uint32_t* indexData = indexBuffer.GetIndexData();
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 3;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 2;
}
