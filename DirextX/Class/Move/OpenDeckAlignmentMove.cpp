#include "OpenDeckAlignmentMove.h"
#include "CardManager.h"

void OpenDeckAlignmentMove::Initialize(CardManager* cardManager) {
	cardManager_ = cardManager;
}

void OpenDeckAlignmentMove::Update() {
	std::vector<Card*> cards = cardManager_->GetZoneCards(CardZone::Open);
	int i = 0;
	int size = static_cast<int>(cards.size()) - 1;
	Vector2 pos{};
	pos.y = 240.0f;
	for (const auto& card : cards) {
		pos.x = 640.0f - (size / 2.0f - i) * 122.0f;
		card->SetNewPos(pos);
		i++;
	}
	isEnd_ = true;
}
