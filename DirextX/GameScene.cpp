#include "GameScene.h"
#include "TextureManager.h"

#include "MathUtility.h"
using namespace MathUtility;
void GameScene::Initialize() {
	TextureManager::GetInstance()->LoadTexture("white.png");
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->AllCardLoad("Card");
	cardManager_->StartCardSet();
}

void GameScene::Update() {
	
}

void GameScene::Draw() {
	
}
