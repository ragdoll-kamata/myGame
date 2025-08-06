#include "AddHandCommand.h"
#include "ErrorMessage.h"
#include "CardManager.h"

bool AddHandCommand::Initialize(const std::string& card) {
	if (card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数じゃないよ");
		return false;
	}
	card_ = card;
	return true; // Initialization successful
}

int AddHandCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Error: card is null
	}
	std::vector<Card*> car = card->GetCards(card_);
	for (Card* c : car) {
		card->GetCardManager()->MoveCard(c, CardZone::Hand);
		//card->RemoveCard(card_, c);
	}
	return 0;
}