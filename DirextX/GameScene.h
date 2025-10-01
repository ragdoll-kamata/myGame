#pragma once
#include "BaseScene.h"
#include <memory>
#include "CardManager.h"

#include "TrunState.h"


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
	TrunState trunState_ = TrunState::Start;
};

