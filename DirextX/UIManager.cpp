#include "UIManager.h"

#include "SpriteCommon.h"
#include "TextCommon.h"

#include "ResourceManager.h"

void UIManager::Initialize(ResourceManager* resourceManager) {
	resourceManager_ = resourceManager;

	// ボタンの初期化
	endTurnButton_ = std::make_unique<Button>();
	endTurnButton_->Initialize({950.0f, 500.0f}, {150.0f, 50.0f}, "white.png", {1.0f, 0.0f, 0.0f, 1.0f});
	endTurnButton_->SetIsDraw(false);

	startOpenButton_ = std::make_unique<Button>();
	startOpenButton_->Initialize({200.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 1.0f, 0.0f, 1.0f});
	startOpenButton_->SetIsDraw(false);

	startOpenEndButton_ = std::make_unique<Button>();
	startOpenEndButton_->Initialize({440.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 0.0f, 1.0f, 1.0f});
	startOpenEndButton_->SetIsDraw(false);

	endSelectButton_ = std::make_unique<Button>();
	endSelectButton_->Initialize({640.0f, 650.0f}, {200.0f, 100.0f}, "white.png", {1.0f, 0.0f, 1.0f, 1.0f});
	endSelectButton_->SetIsDraw(false);

	// フィールドカード用当たり判定初期化
	cardExecutionField_ = std::make_unique<Button>();
	cardExecutionField_->Initialize({1150.0f, 400.0f}, {120.0f * 1.2f, 160.0f * 1.2f}, "white.png", {0.0f, 1.0f, 1.0f, 1.0f});
	cardExecutionField_->SetIsDraw(true);

	for (int i = 0; i < maxFieldCard; i++) {
		FieldCard fi;
		fi.field = std::make_unique<Button>();
		fi.field->Initialize(FieldCardPos(i), {120.0f * 1.1f, 160.0f * 1.1f}, "white.png", {0.4f, 0.9f, 0.4f, 0.9f});
		fi.field->SetIsDraw(true);
		fieldCardField_.push_back(std::move(fi));
	}

	// 背景スプライト初期化
	costBackSprite_ = std::make_unique<Sprite>();
	costBackSprite_->Initialize("white.png");
	costBackSprite_->SetSize({60.0f, 40.0f});
	costBackSprite_->SetPosition({0.0f, 0.0f});
	costBackSprite_->SetColor({0.9f, 0.9f, 0.5f, 1.0f});

	costBackSprite2_ = std::make_unique<Sprite>();
	costBackSprite2_->Initialize("white.png");
	costBackSprite2_->SetSize({60.0f, 40.0f});
	costBackSprite2_->SetPosition({60.0f, 0.0f});
	costBackSprite2_->SetColor({0.5f, 0.0f, 0.5f, 1.0f});

	selectCardBackSprite_ = std::make_unique<Sprite>();
	selectCardBackSprite_->Initialize("white.png");
	selectCardBackSprite_->SetSize({1280.0f, 720.0f});
	selectCardBackSprite_->SetColor({0.0f, 0.0f, 0.0f, 0.0f});

	rampageGaugeBackSprite_ = std::make_unique<Sprite>();
	rampageGaugeBackSprite_->Initialize("white.png");
	rampageGaugeBackSprite_->SetSize({30.0f, 500.0f});
	rampageGaugeBackSprite_->SetPosition({1250.0f, 100.0f});
	rampageGaugeBackSprite_->SetAnchorPoint({0.5f, 0.0f});
	rampageGaugeBackSprite_->SetColor({0.2f, 0.2f, 0.2f, 1.0f});

	// 暴走ゲージスプライト初期化
	rampageGaugeSprite_ = std::make_unique<Sprite>();
	rampageGaugeSprite_->Initialize("white.png");
	rampageGaugeSprite_->SetSize({30.0f, 0.0f});
	rampageGaugeSprite_->SetPosition({1250.0f, 600.0f});
	rampageGaugeSprite_->SetAnchorPoint({0.5f, 1.0f});
	rampageGaugeSprite_->SetColor({1.0f, 0.0f, 1.0f, 1.0f});

	// コスト表示用テキスト初期化
	lightCostText_ = std::make_unique<Text>();
	lightCostText_->Initialize(U"00", {10.0f, 0.0f}, 20000.0f);
	lightCostText_->Update();
	lightCostText_->CalcFitSize(40.0f);

	darknessCostText_ = std::make_unique<Text>();
	darknessCostText_->Initialize(U"00", {70.0f, 0.0f}, 2000.0f);
	darknessCostText_->Update();
	darknessCostText_->CalcFitSize(40.0f);


}
void UIManager::Update() {

	// ボタン更新
	endTurnButton_->Update();
	startOpenButton_->Update();
	startOpenEndButton_->Update();
	endSelectButton_->Update();

	// コスト表示更新
	lightCostText_->Update();
	darknessCostText_->Update();
	costBackSprite_->Updata();
	costBackSprite2_->Updata();

	// フィールドカード用当たり判定更新
	cardExecutionField_->Update();
	for (const auto& fi : fieldCardField_) {
		fi.field->Update();
	}

	// 暴走ゲージ更新
	float gaugeH = (resourceManager_->GetRampageGauge() / resourceManager_->GetMaxRampageGauge()) * 500.0f;
	rampageGaugeSprite_->SetSize({30.0f, gaugeH});
	rampageGaugeSprite_->Updata();
	rampageGaugeBackSprite_->Updata();

	// セレクトバック更新
	if (isSelectCard_) {
		selectCardBackSpriteAlpha += 0.05f;
		if (selectCardBackSpriteAlpha > kSelectCardBackSpriteMaxAlpha) {
			selectCardBackSpriteAlpha = kSelectCardBackSpriteMaxAlpha;
		}
		selectCardBackSprite_->SetColor({0.0f, 0.0f, 0.0f, selectCardBackSpriteAlpha});
	} else {
		selectCardBackSpriteAlpha -= 0.05f;
		if (selectCardBackSpriteAlpha < 0.0f) {
			selectCardBackSpriteAlpha = 0.0f;
		}
		selectCardBackSprite_->SetColor({0.0f, 0.0f, 0.0f, selectCardBackSpriteAlpha});

	}
	selectCardBackSprite_->Updata();
}
void UIManager::Draw() {
	SpriteCommon::GetInstance()->PreDraw();

	// ボタン描画
	endTurnButton_->Draw();
	startOpenButton_->Draw();
	startOpenEndButton_->Draw();

	// コスト表示描画
	costBackSprite_->Draw();
	costBackSprite2_->Draw();
	TextCommon::GetInstance()->PreDraw();
	lightCostText_->Draw();
	darknessCostText_->Draw();
	TextCommon::GetInstance()->PostDraw();

	SpriteCommon::GetInstance()->PreDraw();
	// フィールドカード用当たり判定描画
	cardExecutionField_->Draw();
	for (const auto& fi : fieldCardField_) {
		fi.field->Draw();
	}
	// 暴走ゲージ描画
	rampageGaugeBackSprite_->Draw();
	rampageGaugeSprite_->Draw();

}
void UIManager::SelectDraw() {
	// セレクト描画
	SpriteCommon::GetInstance()->PreDraw();
	selectCardBackSprite_->Draw();
	endSelectButton_->Draw();
}
void UIManager::CostTextUpdate() {
	lightCostText_->SetText(lightCostText_->GetIntToString(resourceManager_->GetLightCost(), 2));
	darknessCostText_->SetText(darknessCostText_->GetIntToString(resourceManager_->GetDarknessCost(), 2));
}

void UIManager::SetEndSelectButtonColorV(float v) {
	endSelectButton_->SetColor({kEndSelectButtonColor.x * v, kEndSelectButtonColor.y * v, kEndSelectButtonColor.z * v, 1.0f});
}

void UIManager::SetEndSelectButtonNormalVector() {
	endSelectButton_->SetPosition({640.0f, 650.0f});
}

void UIManager::SetEndSelectButtonHandVector() {
	endSelectButton_->SetPosition({640.0f, 360.0f});
}

int UIManager::IsOnCollisionFieldCardField(Vector2 pos) {
	for (int i = 0; i < maxFieldCard; i++) {
		if (fieldCardField_[i].field->IsOnCollision(pos)) {
			return i;
		}
	}
	return -1;
}

Vector2 UIManager::GetCardPos(CardZone zone, int index, int size) {
	if (zone == CardZone::Hand) {
		return HandCardPos(index, size);
	} else if (zone == CardZone::Open) {
		return OpenCardPos(index, size);
	} else if (zone == CardZone::Cemetery) {
		return cemeteryCardPos;
	} else if (zone == CardZone::Deck) {
		return deckCardPos;
	} else if (zone == CardZone::Field) {
		return FieldCardPos(index);
	}
	return Vector2();
}

Vector2 UIManager::HandCardPos(int index, int size) {
	Vector2 pos = {};
	pos.x = handCardCenterPos.x - (size / 2.0f - index) * (cardSizeW + handCardPadding);
	pos.y = handCardCenterPos.y;
	return pos;
}
Vector2 UIManager::OpenCardPos(int index, int size) {
	Vector2 pos = {};
	pos.x = openCardCenterPos.x - (size / 2.0f - index) * (cardSizeW + openCardPadding);
	pos.y = openCardCenterPos.y;
	return pos;
}
Vector2 UIManager::FieldCardPos(int i) {
	return {fieldCardCenterPos.x + i * (cardSizeW * 1.1f + fieldCardPadding), fieldCardCenterPos.y};
}
