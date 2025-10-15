#include "HandCardMove.h"
#include "Card.h"
#include "CardManager.h"
#include "DeltaTime.h"


void HandCardMove::Initialize(Card* card, float time) {
	card_ = card;
	time_ = time;
}

void HandCardMove::Update() {
	if (!isStart_) {
		card_->GetCardManager()->HandAdjustment();
		isStart_ = true;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		isEnd_ = true;
		return;
	}
	
}

