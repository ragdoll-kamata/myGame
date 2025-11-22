#include "Card.h"
#include "CardData.h"

#include <MathUtility.h>

using namespace MathUtility;

bool Card::InitializeCard(CardData* loadCard) {
	cardData_ = loadCard;

	name_ = std::make_unique<Text>();
	description_ = std::make_unique<Text>();
	name_->Initialize(U"名前", {0.0f, 0.0f}, 9999.0f);
	name_->SetAnchorPoint({0.5f, 1.0f});
	name_->SetTextFormat(Text::TextFormat::Centor);
	description_->Initialize(U"説明", {5.0f, 45.0f}, 400.0f);
	description_->CalcFitSizeOneHeight(20.0f);
	int dummyLine = 0;
	FunctionResult result = cardData_->CardFunctionLoad(this, "初期設定", dummyLine);
	if (result != FunctionResult::Normal) {
		return false;
	}
	if(dummyLine != 0) {
		// エラー処理
	}
	name_->Update();
	name_->CalcFitSize(120.0f);
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(fileName);
	sprite_->SetAnchorPoint({0.5f, 0.5f});
	sprite_->SetSize(halfSize * 2.0f);
	sprite_->SetPosition(pos);
	if (element_ == CardElement::None) {
		sprite_->SetColor({0.9f, 0.9f, 0.9f, 1.0f});
	}
	if (element_ == CardElement::Light) {
		sprite_->SetColor({0.9f, 0.9f, 0.5f, 1.0f});
	}
	if (element_ == CardElement::Darkness) {
		sprite_->SetColor({0.5f, 0.0f, 0.5f, 1.0f});
	}
	costSprite_ = std::make_unique<Sprite>();
	costSprite_->Initialize("sphere.png");
	costSprite_->SetAnchorPoint({0.5f, 0.5f});
	costSprite_->SetSize({20.0f, 20.0f});
	if (elementCost_ == CardElement::None) {
		costSprite_->SetColor({0.9f, 0.9f, 0.9f, 1.0f});
	}
	if (elementCost_ == CardElement::Light) {
		costSprite_->SetColor({0.9f, 0.9f, 0.5f, 1.0f});
	}
	if (elementCost_ == CardElement::Darkness) {
		costSprite_->SetColor({0.5f, 0.0f, 0.5f, 1.0f});
	}

	costSprite2_ = std::make_unique<Sprite>();
	costSprite2_->Initialize("sphere.png");
	costSprite2_->SetAnchorPoint({0.5f, 0.5f});
	costSprite2_->SetSize({25.0f, 25.0f});
	costSprite2_->SetColor({0.5f, 0.5f, 0.5f, 1.0f});

	costText_ = std::make_unique<Text>();
	costText_->Initialize(Text::GetIntToString(cost_, 1), {0.0f, 0.0f}, 20000.0f);
	costText_->SetAnchorPoint({0.5f, 0.5f});
	costText_->SetTextFormat(Text::TextFormat::Centor);
	costText_->Update();
	costText_->CalcFitSize(10.0f);

	return true;
}

void Card::Update() {
	if (isMove && !isCommandMove_) {
		sprite_->SetPosition(MathUtility::Lerp(sprite_->GetPosition(), pos, 0.15f));
		name_->SetPosition(MathUtility::Lerp(name_->GetPosition(), pos - textZure, 0.15f));
		costSprite_->SetPosition({sprite_->GetPosition().x - halfSize.x + 15.0f, sprite_->GetPosition().y - halfSize.y + 15.0f});
		costSprite2_->SetPosition({sprite_->GetPosition().x - halfSize.x + 15.0f, sprite_->GetPosition().y - halfSize.y + 15.0f});
		costText_->SetPosition({sprite_->GetPosition().x - halfSize.x + 15.0f, sprite_->GetPosition().y - halfSize.y + 15.0f});
		if (MathUtility::Length(sprite_->GetPosition() - pos) <= 1.0f) {
			isMove = false;
			sprite_->SetPosition(pos);
			name_->SetPosition(pos - textZure);
		}
	}
	sprite_->Updata();
	name_->Update();
	costSprite_->Updata();
	costSprite2_->Updata();
	costText_->Update();
	description_->Update();
}

void Card::Draw() {
	if (isDraw_) {
		sprite_->Draw();
		costSprite2_->Draw();
		costSprite_->Draw();
	}
}

void Card::TextDraw() {
	if (isDraw_) {
		name_->Draw();
		costText_->Draw();
	}
}

void Card::EffectTextDraw() {
	if (isDraw_) {
		description_->Draw();
	}
}

bool Card::IsOnCollision(Vector2 pos) {

	Vector2 hPos{
		std::clamp(pos.x, sprite_->GetPosition().x - halfSize.x, sprite_->GetPosition().x + halfSize.x),
		std::clamp(pos.y, sprite_->GetPosition().y - halfSize.y, sprite_->GetPosition().y + halfSize.y),
	};

	hPos.x -= pos.x;
	hPos.y -= pos.y;
	float len = MathUtility::Length(hPos);
	if (len <= 0.0f) {
		return true;
	}
	return false;
}

bool Card::Effect() {
	FunctionResult result = cardData_->CardFunctionLoad(this, "効果", functionLine);
	if(result == FunctionResult::Error) {
		// エラー処理
	}
	if (result == FunctionResult::Standby) {
		return false;
	}
	return true;
}

void Card::SetPos(Vector2& pos) {
	sprite_->SetPosition(pos);
	name_->SetPosition(pos - textZure);
	costSprite_->SetPosition({pos.x - halfSize.x + 15.0f, pos.y - halfSize.y + 15.0f});
	costSprite2_->SetPosition({pos.x - halfSize.x + 15.0f, pos.y - halfSize.y + 15.0f});
	costText_->SetPosition({pos.x - halfSize.x + 15.0f, pos.y - halfSize.y + 15.0f});
}
