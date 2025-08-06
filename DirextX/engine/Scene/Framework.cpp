#include "Framework.h"
#include "Input.h"
#include "Audio.h"
#include "DeltaTime.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "LineManager.h"
#include "SkinCluster.h"
#include "TextCommon.h"

void Framework::Run()
{
	Initialize();

	while (true) {
		Update();

		if (isEnd) {
			break;
		}

		Draw();
	}

	Finalize();
}

void Framework::Initialize()
{
	d3dResourceLeakChecker = std::make_unique<D3DResourceLeakChecker>();
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	directXCommon_ = std::make_unique<DirectXCommon>();;
	directXCommon_->Initialize(winApp_.get());

	srvManager_ = std::make_unique<SrvManager>();;
	srvManager_->Initialize(directXCommon_.get());

	imGuiManager_ = std::make_unique<ImGuiManager>();;
	imGuiManager_->Initialize(winApp_.get(), directXCommon_.get(), srvManager_.get());

	TextureManager::GetInstance()->Initialize(directXCommon_.get(), srvManager_.get());


	Input::GetInstance()->Initialize(winApp_.get());

	Audio::GetInstance()->Initialize();
	
	DeltaTime::GetInstance()->Initialize();

	SpriteCommon::GetInstance()->Initialize(directXCommon_.get());
	Object3dCommon::GetInstance()->Initialize(directXCommon_.get());
	ModelManager::GetInstance()->Initialize(directXCommon_.get());

	Primitive::SetDirectXCommon(directXCommon_.get());
	ParticleManager::GetInstance()->Initialize(directXCommon_.get(), srvManager_.get());

	LineManager::GetInstance()->Initialize(directXCommon_.get(), srvManager_.get());


	SkinCluster::SetDirectXCommon(directXCommon_.get());
	SkinCluster::SetSrvManager(srvManager_.get());
	TextCommon::GetInstance()->Initialize(directXCommon_.get(), srvManager_.get(), "apr.otf", "resource/font");
}

void Framework::Update()
{
	if (winApp_->ProcessMessage()) {
		isEnd = true;
	}
	Input::GetInstance()->Update();

	DeltaTime::GetInstance()->Update();

	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		isEnd = true;
	}
	LineManager::GetInstance()->ClearLines();
}

void Framework::Finalize()
{
	LineManager::GetInstance()->Finalize();

	ParticleManager::GetInstance()->Finalize();

	TextCommon::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	Object3dCommon::GetInstance()->Finalize();
	SpriteCommon::GetInstance()->Finalize();

	DeltaTime::GetInstance()->Finalize();

	Audio::GetInstance()->Finalize();

	Input::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	imGuiManager_->Finalize();

	directXCommon_->Finalize();

	winApp_->Finalize();
	
}
