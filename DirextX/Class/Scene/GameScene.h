#pragma once
#include "BaseScene.h"
#include <memory>

#include "TrunState.h"

#include "CardManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "PlayerInput.h"
#include "ShopManager.h"

class GameScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	std::unique_ptr<CardManager> cardManager_;
	std::unique_ptr<UIManager> uiManager_;
	std::unique_ptr<ResourceManager> resourceManager_;
	std::unique_ptr<ShopManager> shopManager_;
	std::unique_ptr<PlayerInput> playerInput_;
	TurnState trunState_ = TurnState::Start;
};

