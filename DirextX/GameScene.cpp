#include "GameScene.h"

#include "MathUtility.h"
using namespace MathUtility;
void GameScene::Initialize() {
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->AllCardLoad("Card");

}

void GameScene::Update() {
	
}

void GameScene::Draw() {
	
}
