#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Text.h"
#include <memory>

#include "Vector2.h"
class TitleScene : public BaseScene
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
	std::unique_ptr<Sprite> sprite_ = nullptr;

	std::unique_ptr<Text> text_ = nullptr;

	float iii = 0.0f;
	Vector2 mouseMove = {0.0f, 0.0f};
};

