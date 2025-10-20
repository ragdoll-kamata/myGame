#include "HandCardMove.h"
#include "Card.h"
#include "CardManager.h"
#include "DeltaTime.h"


void HandCardMove::Initialize(CardManager* cardManager, Card* card, std::vector<Card*> cards, float time) {
	cardManager_ = cardManager;
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
		cardManager_->HandAdjustment();
		isStart_ = true;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		isEnd_ = true;
		return;
	}
	
}


