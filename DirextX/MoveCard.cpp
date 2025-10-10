#include "MoveCard.h"
#include "Card.h"
#include "DeltaTime.h"

using namespace MathUtility;

void MoveCardCo::Initialize(Card* card, Vector2 pos, float time) {
	card_ = card;
	pos_ = pos;
	time_ = time;
	nowTime_ = 0.0f;
	isEnd_ = false;
	startPos_ = card_->GetPos();
}

void MoveCardCo::Update() {
	if (isEnd_) {
		return;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		card_->SetPos(pos_);
		isEnd_ = true;
		card_->SetIsMove(false);
		return;
	}
	float t = nowTime_ / time_;
	Vector2 newPos = Lerp(startPos_, pos_, t);
	card_->SetPos(newPos);
}