#pragma once
#include <string>
#include <list>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>
#include <unordered_map>
#include <random>
#include <functional>

#include "Vector2.h"
#include "Vector4.h"

#include "Matrix4x4.h"
#include "Particle.h"
#include "Primitive.h"

#include "EffectSpawnParams.h"
#include "RandomRotateType.h"
#include "EmissionShape.h"

class DirectXCommon;
class SrvManager;

class ParticleManager
{
public:
	struct Material {
		Vector4 color;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 uvTransform;
		Vector4 color;
	};
	struct TextureData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};
	struct MaterialData {
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
		Material* materialData = nullptr;
	};
	struct InstancingData {
		Microsoft::WRL::ComPtr <ID3D12Resource> instancingResource;
		TransformationMatrix* wvpData = nullptr;
		uint32_t instancingSrvIndex = 0;
		uint32_t instanceNum = 0;
	};
	struct ParticleGroup {
		TextureData textureData;
		MaterialData materialData;
		InstancingData instancingData;
		std::list<Particle> particleList;
		std::shared_ptr<Primitive> primitive;
		std::function<void(Particle& particle, float& deltaTime)> moveFunc;
		std::function<void(float& t)> easingFunc;
		bool isBillboardMatrix = false;
		bool isTimer = true;
	};

	void Initialize(DirectXCommon* directXCommon, SrvManager* srvManager);

	/// <summary>
	/// パーティクルグループの作成(同じ名前で作ろうとしたらassertされるぞ！)
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureGalePath">テクスチャ</param>
	/// <param name="primitive">プリミティブ(使いたいのをPrimitiveのstatic関数から選んで入れればOK)(入れないなら板ポリになる)</param>
	/// <param name="isTimer">時間経過で消えるか(入れなければtrue)</param>
	/// <param name="isBillboardMatrix">カメラの方を向き続けるか(入れなければtrue)</param>
	void CreateParticleGroup(const std::string name, const std::string textureGilePath, std::shared_ptr<Primitive> primitive = Primitive::CreatePlane(), bool isTimer = true, bool isBillboardMatrix = true);

	void SetMoveFunc(const std::string& name, std::function<void(Particle& particle, float& deltaTime)> func = nullptr);
	void SetEasingFunc(const std::string& name, std::function<void(float& t)> func);

	/// <summary>
	/// パーティクルを手動で生成したいときに使う(パーティクルグループ名間違えたら何も起きません)
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="particle">パーティクル</param>
	void AddParticle(const std::string name, Particle particle);

	/// <summary>
	/// パーティクルのクリア (パーティクルグループ名間違えたら何も起きません)
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	void ClearParticle(const std::string name) {
		particleGroups[name]->particleList.clear();
	}

	/// <summary>
	/// パーティクルグループの消去 (パーティクルグループ名間違えたら何も起きません)
	/// </summary>
	/// <param name="name">消去したいパーティクルグループ名</param>
	void ClearParticleGroup(const std::string name) {
		particleGroups.erase(name);
	}

	/// <summary>
	/// パーティクルグループのリセット
	/// </summary>
	void AllClearParticleGroup() {
		particleGroups.clear();
	}

	void Update();

	void Draw();

	void Emit(EffectSpawnParams effectSpawnParams, EmissionShape* emissionShape);

	static ParticleManager* GetInstance();
	void Finalize();
private:
	void CreateRootSignature();
	void CreateGraphicsPipeline();
	void CreateMaterial(MaterialData& materialData);
	Vector3 RandomRotate(RandomRotateType randomRotateType);
private:

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;


	std::unordered_map<std::string, std::unique_ptr<ParticleGroup>> particleGroups;

	// 借りたポインタ
	DirectXCommon* directXCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	// 1つのインスタンスあたりの最大数
	const uint32_t kMaxInstance = 200;


	Vector2 anchorPoint_ = { 0.5f,0.5f };

	std::random_device seedGenerator;
	std::mt19937 randomEngine;

	Matrix4x4 backToFrontMatrix = {};

private:
	static ParticleManager* instance;
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = default;
	ParticleManager& operator=(ParticleManager&) = default;
};

