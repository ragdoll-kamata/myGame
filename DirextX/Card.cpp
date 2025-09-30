#include "Card.h"
#include "LoadCard.h"

bool Card::InitializeCard(LoadCard* loadCard) {
	if (!loadCard->CardFunctionLoad(this, "初期設定")) {
		return false;
	}
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(fileName);
	sprite_->SetAnchorPoint({0.5f, 0.5f});
	sprite_->SetSize({120.0f, 160.0f});
	return true;
}

void Card::Draw() {
	if (isDraw_) {
		sprite_->Draw();
	}
}

void Card::TextDraw() {

}
