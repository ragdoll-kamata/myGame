#include "OpenDeckCommand.h"
#include "CardManager.h"
#include "ElementFilterCommand.h"
#include "ErrorMessage.h"
#include "CardMove.h"

bool OpenDeckCommand::Initialize(std::string num, std::string card) {
	if (card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	if (num.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"中身ないよ");
		return false; // Error: num is empty
	}
	if (num.front() != '#') {
		std::unique_ptr<Card> cardptr = std::make_unique<Card>();
		int number = ParseInt(num, cardptr.get());
		if (number < 0) {
			ErrorMessage::GetInstance()->SetMessage(U"数字に変換できないよ");
			return false;
		}
	}
	num_ = num;	
	card_ = card;
	return true; // Initialization successful
}

int OpenDeckCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Error: card is null
	}
	int number = ParseInt(num_, card);
	std::vector<Card*> ca = card->GetCardManager()->OpenDeck(number, true);
	int i = 0;
	std::vector<std::unique_ptr<CardMove>> moves;
	for (Card* c : ca) {
		Vector2 pos = card->GetCardManager()->GetCardPos(CardZone::Open, i);
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(c, pos, 0.5f, true);
		moves.push_back(std::move(move));
		card->AddCard(card_, c);
		c->SetIsDraw(true);
		i++;
	}
	card->GetCardManager()->AddCardMove(std::move(moves));
	return 0;
}

