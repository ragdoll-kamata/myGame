#include "HandCardMove.h"
#include "Card.h"
#include "CardManager.h"
#include "DeltaTime.h"


void HandCardMove::Initialize(Card* card, std::vector<Card*> cards, float time) {
	card_ = card;
	cards_ = cards;
	time_ = time;
	for(const auto& card : cards_){
		card->SetIsCommandMove(true);
	}
}

void HandCardMove::Update() {
	if (!isStart_) {
		for (const auto& card : cards_) {
			card->SetIsCommandMove(false);
		}
		card_->GetCardManager()->HandAdjustment();
		isStart_ = true;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		isEnd_ = true;
		return;
	}
	
}


