#include "AddCemeteryCommand.h"
#include "ErrorMessage.h"
#include "CardManager.h"
#include "CardMove.h"

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
	std::vector<std::unique_ptr<CardMove>> moves;
	for (Card* c : car) {
		Vector2 pos = cardManager_->GetCardPos(CardZone::Cemetery, 0);
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(c, pos, 0.5f, true);
		moves.push_back(std::move(move));
		cardManager_->MoveCard(c, CardZone::Cemetery);
	}
	cardManager_->AddCardMove(std::move(moves));
	return 0;
}
