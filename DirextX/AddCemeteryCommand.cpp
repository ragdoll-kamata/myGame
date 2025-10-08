#include "AddCemeteryCommand.h"
#include "ErrorMessage.h"
#include "CardManager.h"

bool AddCemeteryCommand::Initialize(const std::string& card) {
	if (card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数じゃないよ");
		return false;
	}
	card_ = card;
	return true; // Initialization successful
}

int AddCemeteryCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Error: card is null
	}
	std::vector<Card*> car = card->GetCards(card_);
	for (Card* c : car) {
		c->SetIsDraw(false);
		card->GetCardManager()->MoveCard(c, CardZone::Cemetery);
	}
	return 0;
}
