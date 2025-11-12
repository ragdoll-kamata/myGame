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
	description_->Initialize(U"説明", {0.0f, 20.0f}, 120.0f);
	int dummyLine = 0;
	if (!cardData_->CardFunctionLoad(this, "初期設定", dummyLine)) {
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

	return true;
}

void Card::Update() {
	if (isMove && !isCommandMove_) {
		sprite_->SetPosition(MathUtility::Lerp(sprite_->GetPosition(), pos, 0.15f));
		name_->SetPosition(MathUtility::Lerp(name_->GetPosition(), pos - textZure, 0.15f));
		if (MathUtility::Length(sprite_->GetPosition() - pos) <= 1.0f) {
			isMove = false;
			sprite_->SetPosition(pos);
			name_->SetPosition(pos - textZure);
		}
	}
	sprite_->Updata();
	name_->Update();
}

void Card::Draw() {
	if (isDraw_) {
		sprite_->Draw();
	}
}

void Card::TextDraw() {
	if (isDraw_) {
		name_->Draw();
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
	if(!cardData_->CardFunctionLoad(this, "効果", functionLine)) {
		// エラー処理
	}
	if (functionLine != 0) {
		return false;
	}
	return true;
}

void Card::SetPos(Vector2& pos) {
	sprite_->SetPosition(pos);
	name_->SetPosition(pos - textZure);
}
