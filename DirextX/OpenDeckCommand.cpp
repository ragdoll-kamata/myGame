#include "OpenDeckCommand.h"
#include "CardManager.h"
#include "ElementFilterCommand.h"
#include "ErrorMessage.h"

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
	std::vector<Card*> ca = card->GetCardManager()->OpenDeck(number);
	for (Card* c : ca) {
		card->AddCard(card_, c);
	}
	return 0;
}

