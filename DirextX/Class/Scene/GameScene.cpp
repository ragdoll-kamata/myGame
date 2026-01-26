#include "GameScene.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "TextCommon.h"
#include "CardCommand.h"
#include "CardMove.h"
#include "SceneManager.h"
#include "Input.h"

#include "MathUtility.h"
using namespace MathUtility;
void GameScene::Initialize() {
	resourceManager_ = std::make_unique<ResourceManager>();
	
	uiManager_ = std::make_unique<UIManager>();
	uiManager_->Initialize(resourceManager_.get());

	cardManager_ = std::make_unique<CardManager>();
	cardManager_->Initialize(uiManager_.get(), resourceManager_.get());
	cardManager_->AllCardLoad("Card");
	cardManager_->StartCardSet();

	shopManager_ = std::make_unique<ShopManager>();
	shopManager_->Initialize(cardManager_.get(), uiManager_.get());

	playerInput_ = std::make_unique<PlayerInput>();
	playerInput_->Initialize(cardManager_.get(), shopManager_.get(), uiManager_.get(), resourceManager_.get());

	CardCommand::SetCardManager(cardManager_.get());
	CardMove::SetCatdManager(cardManager_.get());
	trunState_ = TurnState::Start;
}

void GameScene::Update() {
	playerInput_->Update(trunState_);
	cardManager_->Update(trunState_);
	uiManager_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_F1)) {
		SceneManager::GetInstance()->SetNextScene("GameScene");
	}
	if (trunState_ == TurnState::Shop) {
		shopManager_->Update(trunState_);
	}
}

void GameScene::Draw() {

	uiManager_->Draw();
	cardManager_->Draw();

	uiManager_->SelectDraw();
	cardManager_->SelectDraw();
	if (trunState_ == TurnState::Shop) {
		shopManager_->Draw();
		uiManager_->ShopDraw();
	}
	cardManager_->EffectTextDraw();
}
