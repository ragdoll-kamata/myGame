#include "CardShuffleMove.h"
#include "CardManager.h"
#include "DeltaTime.h"
#include "Audio.h"

void CardShuffleMove::Initialize(CardManager* cardManager, std::vector<Card*> shuffleCards, int SH, float time) {
	cardManager_ = cardManager;
	time_ = time;
	shuffleCards_ = shuffleCards;
	SH_ = SH;
	nowTime_ = 0.0f;
	isEnd_ = false;
	isStart_ = false;
}

void CardShuffleMove::Update() {
	if (!isStart_) {

		Audio::GetInstance()->SoundPlayWave(SH_, false);
		isStart_ = true;
	}
	nowTime_ += DeltaTime::GetInstance()->GetDeltaTime();
	if (nowTime_ >= time_) {
		Vector2 pos = cardManager_->GetCardPos(CardZone::Deck, 0);
		for (Card* c : shuffleCards_) {
			c->SetPos(pos);
		}
		isEnd_ = true;
		return;
	}
}
