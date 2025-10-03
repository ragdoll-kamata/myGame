#include "Card.h"
#include "LoadCard.h"

#include <MathUtility.h>

using namespace MathUtility;

bool Card::InitializeCard(LoadCard* loadCard) {
	name_ = std::make_unique<Text>();
	description_ = std::make_unique<Text>();
	name_->Initialize(U"名前", {0.0f, 0.0f}, 9999.0f);
	name_->SetAnchorPoint({0.5f, 0.0f});
	name_->SetTextFormat(Text::TextFormat::Centor);
	description_->Initialize(U"説明", {0.0f, 20.0f}, 120.0f);
	if (!loadCard->CardFunctionLoad(this, "初期設定")) {
		return false;
	}
	name_->Update();
	name_->CalcFitSize(120.0f);
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(fileName);
	sprite_->SetAnchorPoint({0.5f, 0.5f});
	sprite_->SetSize({120.0f, 160.0f});

	return true;
}

void Card::Update() {
	if (isMove) {
		sprite_->SetPosition(MathUtility::Lerp(sprite_->GetPosition(), pos, 0.3f));
		name_->SetPosition(MathUtility::Lerp(name_->GetPosition(), pos - textZure, 0.3f));
		if (MathUtility::Length(sprite_->GetPosition() - pos) <= 0.1f) {
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

void Card::SetPos(Vector2& pos) {
	sprite_->SetPosition(pos);
	name_->SetPosition(pos - textZure);
}
