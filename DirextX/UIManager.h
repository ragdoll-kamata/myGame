#pragma once
#include <memory>
#include "Button.h"
#include "Sprite.h"
#include "Text.h"
#include "Card.h"

struct FieldCard {
	std::unique_ptr<Button> field;
	bool isOn = false;
};

class ResourceManager;

class UIManager {
public:
	void Initialize(ResourceManager* resourceManager);
	void Update();
	void Draw();
	void ShopDraw();
	void SelectDraw();

	void CostTextUpdate();

	void SetEndTurnButtonIsDraw(bool isDraw) {
		endTurnButton_->SetIsDraw(isDraw);
	}
	void SetStartOpenButtonIsDraw(bool isDraw) {
		startOpenButton_->SetIsDraw(isDraw);
	}
	void SetStartOpenEndButtonIsDraw(bool isDraw) {
		startOpenEndButton_->SetIsDraw(isDraw);
	}
	void SetRerollShopButtonIsDraw(bool isDraw) {
		rerollShopButton_->SetIsDraw(isDraw);
	}
	void SetEndShopButtonIsDraw(bool isDraw) {
		endShopButton_->SetIsDraw(isDraw);
	}
	void SetIsSelectCard(bool isSelect) {
		isSelectCard_ = isSelect;
		if (!isSelect) {
			selectCardBackSpriteAlpha = 0.0f;
		}
	}


	void SetEndSelectButtonIsDraw(bool isDraw) {
		endSelectButton_->SetIsDraw(isDraw);
	}
	void SetEndSelectButtonColor(Vector4 color) {
		endSelectButton_->SetColor(color);
	}
	void SetEndSelectButtonPosition(Vector2 pos) {
		endSelectButton_->SetPosition(pos);
	}
	void SetEndSelectButtonColorV(float v);
	void SetEndSelectButtonNormalVector();
	void SetEndSelectButtonHandVector();


	void SetFieldCardFieldIsOn(int index, bool isOn) {
		fieldCardField_[index].isOn = isOn;
	}
	bool GetFieldCardFieldIsOn(int index) const {
		return fieldCardField_[index].isOn;
	}

	// 当たり判定

	bool IsOnCollisionEndTurnButton(Vector2 pos) {
		return endTurnButton_->IsOnCollision(pos);
	}
	bool IsOnCollisionStartOpenButton(Vector2 pos) {
		return startOpenButton_->IsOnCollision(pos);
	}
	bool IsOnCollisionStartOpenEndButton(Vector2 pos) {
		return startOpenEndButton_->IsOnCollision(pos);
	}
	bool IsOnCollisionEndSelectButton(Vector2 pos) {
		return endSelectButton_->IsOnCollision(pos);
	}
	bool IsOnCollisionExecutionField(Vector2 pos) {
		return cardExecutionField_->IsOnCollision(pos);
	}
	bool IsOnCollisionRerollShopButton(Vector2 pos) {
		return rerollShopButton_->IsOnCollision(pos);
	}
	bool IsOnCollisionEndShopButton(Vector2 pos) {
		return endShopButton_->IsOnCollision(pos);
	}

	int IsOnCollisionFieldCardField(Vector2 pos);

	Vector2 GetCardPos(CardZone cardZone, int index, int size);

private:
	Vector2 HandCardPos(int index, int size);
	Vector2 OpenCardPos(int index, int size);
	Vector2 FieldCardPos(int i);
private:
	ResourceManager* resourceManager_ = nullptr;

	// ボタン
	std::unique_ptr<Button> endTurnButton_ = nullptr;

	std::unique_ptr<Button> startOpenButton_ = nullptr;
	std::unique_ptr<Button> startOpenEndButton_ = nullptr;

	std::unique_ptr<Button> endSelectButton_ = nullptr;
	const Vector4 kEndSelectButtonColor = {0.0f, 0.5f, 1.0f, 1.0f};
	float selectCardBackSpriteAlpha = 0.0f;
	const float kSelectCardBackSpriteMaxAlpha = 0.5f;

	std::unique_ptr<Button> rerollShopButton_ = nullptr;
	std::unique_ptr<Button> endShopButton_ = nullptr;

	// カード実行Field
	std::unique_ptr<Button> cardExecutionField_ = nullptr;
	std::vector<FieldCard> fieldCardField_;
	const int maxFieldCard = 5;


	// 暴走ゲージ用スプライト
	std::unique_ptr<Sprite> rampageGaugeBackSprite_ = nullptr;
	std::unique_ptr<Sprite> rampageGaugeSprite_ = nullptr;

	// コスト表示用背景スプライト
	std::unique_ptr<Sprite> costBackSprite_ = nullptr;
	std::unique_ptr<Sprite> costBackSprite2_ = nullptr;

	// コスト表示用テキスト
	std::unique_ptr<Text> lightCostText_ = nullptr;
	std::unique_ptr<Text> darknessCostText_ = nullptr;

	// 所持金表示用テキスト
	std::unique_ptr<Text> moneyText_ = nullptr;

	// スコア表示用テキスト
	std::unique_ptr<Text> scoreText_ = nullptr;

	// カード選択中かどうか
	bool isSelectCard_ = false;
	std::unique_ptr<Sprite> selectCardBackSprite_ = nullptr;

	// ショップ背景スプライト
	std::unique_ptr<Sprite> shopBackSprite_ = nullptr;




	// カードゾーンごとの所定中央位置

	const Vector2 openCardCenterPos = {640.0f, 140.0f};
	const Vector2 handCardCenterPos = {640.0f, 720.0f - 80.0f};
	const Vector2 fieldCardCenterPos = {200.0f, 360.0f};
	const Vector2 cemeteryCardPos = {-150.0f, 80.0f};
	const Vector2 deckCardPos = {640.0f, -160.0f};
	// カードサイズ
	const float cardSizeW = 120.0f;

	// カード間のパディング
	const float openCardPadding = 10.0f;
	const float handCardPadding = 5.0f;
	const float fieldCardPadding = 20.0f;
};

