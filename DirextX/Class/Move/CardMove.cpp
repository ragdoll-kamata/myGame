#include "CardMove.h"
#include "Card.h"
#include "DeltaTime.h"

using namespace MathUtility;

CardMove::~CardMove() {
}

void CardMove::Initialize(Card* card, Vector2 pos, float time, float isEndDraw, float startTime) {
	card_ = card;
	pos_ = pos;
	time_ = time;
	startTime_ = startTime;
	isEndDraw_ = isEndDraw;
	nowTime_ = 0.0f;
	isEnd_ = false;
	startPos_ = card_->GetPos();
	//card_->SetIsCommandMove(true);
}

void CardMove::Update() {
	if(startTime_ > 0.0f){
		startTime_ -= DeltaTime::GetInstance()->GetDeltaTime();
		card_->SetIsMove(false);
		card_->SetIsDraw(true);
		return;
	}
	card_->SetIsMove(false);
	card_->SetIsDraw(true);
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
	float t2 = 1.0f - t;
	Vector2 newPos = Lerp(startPos_, pos_, 1.0f - t2 * t2 * t2);
	card_->SetPos(newPos);
}

void CardMove::SetStart() {
	if(card_ == nullptr){
		return;
	}
	startPos_ = card_->GetPos();
}

void CardMove::End() {
	if (card_ == nullptr) {
		return;
	}
	card_->SetIsCommandMove(false);
}
