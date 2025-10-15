#include "CardMove.h"
#include "Card.h"
#include "DeltaTime.h"

using namespace MathUtility;

CardMove::~CardMove() {
}

void CardMove::Initialize(Card* card, Vector2 pos, float time, float isEndDraw) {
	card_ = card;
	pos_ = pos;
	time_ = time;
	isEndDraw_ = isEndDraw;
	nowTime_ = 0.0f;
	isEnd_ = false;
	startPos_ = card_->GetPos();
}

void CardMove::Update() {
	if (isEnd_) {
		return;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		card_->SetPos(pos_);
		isEnd_ = true;
		card_->SetIsMove(false);
		card_->SetIsDraw(isEndDraw_);
		return;
	}
	float t = nowTime_ / time_;
	float t2 =  1.0f -t;
	Vector2 newPos = Lerp(startPos_, pos_, 1.0f - t2 * t2 * t2);
	card_->SetPos(newPos);
}

void CardMove::SetStart() {
	startPos_ = card_->GetPos();
}
