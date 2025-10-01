#include "GameScene.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "TextCommon.h"

#include "MathUtility.h"
using namespace MathUtility;
void GameScene::Initialize() {
	TextureManager::GetInstance()->LoadTexture("white.png");
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->Initialize();
	cardManager_->AllCardLoad("Card");
	cardManager_->StartCardSet();
}

void GameScene::Update() {
	cardManager_->Update(trunState_);
}

void GameScene::Draw() {
	SpriteCommon::GetInstance()->PreDraw();

	cardManager_->Draw();

	//TextCommon::GetInstance()->PreDraw();

	//cardManager_->TextDraw();
	
}
