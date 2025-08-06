#include "ElementFilterCommand.h"
#include "ErrorMessage.h"

bool ElementFilterCommand::Initialize(std::string element, std::string card, std::string filterCard) {
	element_ = ParseCardElement(element);
	if (element_ == CardElement::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"属性がおかしいよ");
		return false;
	}
	if (filterCard.front() != '$' || card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	card_ = card;
	filterCard_ = filterCard;

	return true;
}

int ElementFilterCommand::Execute(Card* card) {
	std::vector<Card*> cards = card->GetCards(card_);
	for(Card* c : cards) {
		if (c->GetElement() == element_) {
			card->AddCard(filterCard_, c);
			card->RemoveCard(card_, c);
		}
	}
	return 0;
}