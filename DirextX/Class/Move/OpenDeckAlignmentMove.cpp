#include "OpenDeckAlignmentMove.h"
#include "CardManager.h"
#include "UIManager.h"

void OpenDeckAlignmentMove::Update() {
	std::vector<Card*> cards = cardManager_->GetZoneCards(CardZone::Open);
	int i = 0;
	int size = static_cast<int>(cards.size()) - 1;
	Vector2 pos{};
	pos.y = 240.0f;
	for (const auto& card : cards) {
		pos = cardManager_->GetUIManager()->GetCardPos(CardZone::Open, i, size);
		card->SetNewPos(pos);
		i++;
	}
	cardManager_->GetUIManager()->CostTextUpdate();
	isEnd_ = true;
}
