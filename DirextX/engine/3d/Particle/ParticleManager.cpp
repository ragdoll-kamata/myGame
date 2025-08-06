#include "ParticleManager.h"
#include "TextureManager.h"
#include <cassert>
#include "SrvManager.h"
#include "MathUtility.h"
#include "Logger.h"
#include "DeltaTime.h"
#include "CameraManager.h"
#include <numbers>

using namespace MathUtility;

ParticleManager* ParticleManager::instance = nullptr;
void ParticleManager::Initialize(DirectXCommon* directXCommon, SrvManager* srvManager)
{
	directXCommon_ = directXCommon;
	srvManager_ = srvManager;

	randomEngine.seed(seedGenerator());

	CreateRootSignature();
	CreateGraphicsPipeline();

	backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
}
void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath, std::shared_ptr<Primitive> primitive, bool isTimer, bool isBillboardMatrix)
{
	assert(!particleGroups.contains(name));

	
	particleGroups.insert(std::make_pair(name, std::make_unique<ParticleGroup>()));
	ParticleGroup* particleGroup = particleGroups[name].get();

	particleGroup->primitive = primitive;
	particleGroup->isTimer = isTimer;
	particleGroup->isBillboardMatrix = isBillboardMatrix;

	particleGroup->textureData.textureFilePath = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	particleGroup->textureData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);


	particleGroup->instancingData.instancingResource = directXCommon_->CreateBufferResource(sizeof(TransformationMatrix) * kMaxInstance);
	particleGroup->instancingData.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&particleGroup->instancingData.wvpData));
	*particleGroup->instancingData.wvpData =
		TransformationMatrix{
			.WVP{MakeIdentity4x4()},
			.uvTransform{MakeIdentity4x4()},
		};

	particleGroup->instancingData.instancingSrvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforStructureBuffer(particleGroup->instancingData.instancingSrvIndex, particleGroup->instancingData.instancingResource.Get(), kMaxInstance, sizeof(TransformationMatrix));


	CreateMaterial(particleGroup->materialData);
}

void ParticleManager::SetMoveFunc(const std::string& name, std::function<void(Particle& particle, float& deltaTime)> func) {
	assert(particleGroups.contains(name));
	particleGroups[name]->moveFunc = func;
}

void ParticleManager::SetEasingFunc(const std::string& name, std::function<void(float& t)> func) {
	assert(particleGroups.contains(name));
	particleGroups[name]->easingFunc = func;
}

void ParticleManager::AddParticle (const std::string name, Particle particle) {
	if (!particleGroups.contains (name)) {
		Logger::Log ("ParticleGroup not found");
		return;
	}
	particleGroups[name]->particleList.push_back(particle);
}

void ParticleManager::Update()
{
	float deltaTime = DeltaTime::GetInstance()->GetDeltaTime();

	backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = backToFrontMatrix *  CameraManager::GetInstance()->GetActiveCamera()->GetWorldTransform();
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	
	for (const auto& [name, particleGroup] : particleGroups) {
		particleGroup->instancingData.instanceNum = 0;
		std::list<decltype(particleGroup->particleList)::iterator> erase; // 消去リスト
		//particleGroup->primitive->SetUVScroll(particleGroup->primitive->GetUVScroll() + Vector2(0.01f, 0.0f));
		particleGroup->primitive->Update();
		// パーティクルの更新
		for (auto j = particleGroup->particleList.begin(); j != particleGroup->particleList.end(); j++) {
			if (particleGroup->isTimer) {
				j->currentTime += deltaTime;

				if (j->lifeTime <= j->currentTime) {
					erase.push_back (j);
					continue;
				}
			}
			if (particleGroup->moveFunc) {
				particleGroup->moveFunc(*j, deltaTime);
			}

			j->transform.translate += j->velocity * deltaTime;
			//j->transform.rotare.y += 0.1f;

			Matrix4x4 scaleMatrix = MakeScaleMatrix(j->transform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix(j->transform.translate);

			// ワールド行列の計算
			Matrix4x4 world = {};
			if (particleGroup->isBillboardMatrix) {
				Matrix4x4 rotateMatrix = MakeRotateZMatrix(j->transform.rotate.z);

				world = scaleMatrix * billboardMatrix * rotateMatrix * translateMatrix;
			} else {
				world = MakeAffineMatrix(j->transform.scale, j->transform.rotate, j->transform.translate);
			}

			Vector4 c = {1.0f, 1.0f, 1.0f, 1.0f};
			float t = 1.0f - (j->currentTime / j->lifeTime);
			particleGroup->easingFunc(t);
			c.w = t;
			
			// ワールド行列をセット
			particleGroup->instancingData.wvpData[particleGroup->instancingData.instanceNum].WVP = world * CameraManager::GetInstance()->GetCameraViewProjectionMatrix();
			particleGroup->instancingData.wvpData[particleGroup->instancingData.instanceNum].uvTransform = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, Vector3{j->uvRotate.x, j->uvRotate.y, 0.0f}, {j->uvScroll.x, j->uvScroll.y, 0.0f});
			particleGroup->instancingData.wvpData[particleGroup->instancingData.instanceNum].color = j->color * c;
			particleGroup->instancingData.instanceNum++;
		}
		// 消去リストのパーティクルを消す
		for (auto e : erase) {
			particleGroup->particleList.erase(e);
		}
	}
}

void ParticleManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(graphicsPipelineState.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	//commandList->IASetIndexBuffer(&indexBufferView);
	for (const auto& [name, particleGroup] : particleGroups) {
		commandList->SetGraphicsRootConstantBufferView(0, particleGroup->materialData.materialResource->GetGPUVirtualAddress());
		commandList->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUdescriptorHandle(particleGroup->textureData.textureIndex));
		commandList->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUdescriptorHandle(particleGroup->instancingData.instancingSrvIndex));
		if(particleGroup->instancingData.instanceNum) {
			particleGroup->primitive->Draw(particleGroup->instancingData.instanceNum);
		}
	}
}

void ParticleManager::Emit(EffectSpawnParams effectSpawnParams, EmissionShape* emissionShape)
{
	assert(particleGroups.contains(effectSpawnParams.name));
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
	std::uniform_real_distribution<float> dis2(0.0f, 1.0f);
	for (uint32_t i = 0; i < effectSpawnParams.count; i++) {
		Particle a{};
		a.transform = effectSpawnParams.transform;
		a.transform.translate += emissionShape->EmitPosition(randomEngine);
		a.transform.rotate = RandomRotate(effectSpawnParams.randomRotateType);
		a.lifeTime = effectSpawnParams.lifeTime;
		a.velocity = { dis(randomEngine),dis(randomEngine) ,dis(randomEngine) };
		a.velocity = MathUtility::Normalize(a.velocity);
		a.velocity *= MathUtility::Lerp(effectSpawnParams.minSpeed, effectSpawnParams.maxSpeed, dis2(randomEngine));
		particleGroups[effectSpawnParams.name]->particleList.push_back(a);
	}


}

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Finalize() {
	if (instance) {
		delete instance;		
	}
}

void ParticleManager::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	rootParameters[1].Descriptor.ShaderRegister = 1;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootsignature.pParameters = rootParameters;
	descriptionRootsignature.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
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

void ParticleManager::CreateGraphicsPipeline()
{
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
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;



	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = 
		directXCommon_->CompileShader(L"Resource/Shaders/Particle.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = 
		directXCommon_->CompileShader(L"Resource/Shaders/Particle.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDescs;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };
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
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;



	//
	HRESULT hr = directXCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void ParticleManager::CreateMaterial(MaterialData& materialData) {
	materialData.materialResource = directXCommon_->CreateBufferResource(sizeof(Material));

	materialData.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData.materialData));

	*materialData.materialData =
		Material{
			.color{1.0f, 1.0f, 1.0f, 1.0f},
		};

}



Vector3 ParticleManager::RandomRotate(RandomRotateType randomRotateType) {
	Vector3 result = {};
	std::uniform_real_distribution<float> dis(0.0f, MathUtility::kPi * 2.0f);
	if (randomRotateType & RandomRotateType::X) {
		result.x = dis(randomEngine);
	}
	if (randomRotateType & RandomRotateType::Y) {
		result.y = dis(randomEngine);
	}
	if (randomRotateType & RandomRotateType::Z) {
		result.z = dis(randomEngine);
	}
	return result;
}
