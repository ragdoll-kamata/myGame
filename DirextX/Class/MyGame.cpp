#include "MyGame.h"
#include "Scene/SceneFactory.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->Initialize(std::make_unique<SceneFactory>());
	sceneManager_->SetNextScene("GameScene");
}

void MyGame::Update()
{
	Framework::Update();
	imGuiManager_->Begin();

	sceneManager_->Update();

	imGuiManager_->End();
}

void MyGame::Draw()
{
	directXCommon_->PreDraw();
	srvManager_->PreDraw();

	sceneManager_->Draw();

	imGuiManager_->Draw();

	directXCommon_->PostDraw();
}

void MyGame::Finalize()
{
	sceneManager_->Finalize();
	Framework::Finalize();
}
