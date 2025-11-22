#include "GameScene.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "TextCommon.h"
#include "CardCommand.h"
#include "SceneManager.h"
#include "Input.h"

#include "MathUtility.h"
using namespace MathUtility;
void GameScene::Initialize() {
	TextureManager::GetInstance()->LoadTexture("white.png");
	cardManager_ = std::make_unique<CardManager>();
	CardCommand::SetCardManager(cardManager_.get());
	cardManager_->Initialize();
	cardManager_->AllCardLoad("Card");
	cardManager_->StartCardSet();
}

void GameScene::Update() {
	cardManager_->Update(trunState_);
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		SceneManager::GetInstance()->SetNextScene("GameScene");
	}
}

void GameScene::Draw() {

	cardManager_->Draw();

}
