#include "AddDeckBottomCommand.h"
#include "CardManager.h"
#include "UIManager.h"
#include "ErrorMessage.h"

bool AddDeckBottomCommand::Initialize(const std::string& card) {
	if (card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	card_ = card;
	return true; // Initialization successful
}

ExecuteResult AddDeckBottomCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Error: card is null
	}
	std::vector<Card*> cards;
	if (!ParseCard(card_, cards, card)) {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数が見つからないよ");
		return ExecuteResult::Error; // Error: could not parse card variable
	}
	cardManager_->ShuffleCards(cards);
	std::vector<std::unique_ptr<CardMove>> moves;
	for (Card* c : cards) {
		Vector2 pos = cardManager_->GetUIManager()->GetCardPos(CardZone::Deck, 0 , 0);
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(c, pos, 0.5f, false);
		moves.push_back(std::move(move));
		cardManager_->MoveCard(c, CardZone::Deck);
	}
	cardManager_->AddCardMove(std::move(moves));
	return ExecuteResult::Normal;
}
