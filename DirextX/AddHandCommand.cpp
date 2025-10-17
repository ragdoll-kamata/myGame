#include "AddHandCommand.h"
#include "ErrorMessage.h"
#include "CardManager.h"
#include "HandCardMove.h"

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
	}
	std::unique_ptr<HandCardMove> move = std::make_unique<HandCardMove>();
	float time = 0.4f;
	if (car.size() <= 0) {
		time = 0.0f;
	}
	move->Initialize(card, car, time);
	std::vector<std::unique_ptr<CardMove>> moves;
	moves.push_back(std::move(move));
	card->GetCardManager()->AddCardMove(std::move(moves));

	return 0;
}