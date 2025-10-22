#include "CardCostCommand.h"
#include "ErrorMessage.h"

bool CardCostCommand::Initialize(const std::string& element, const std::string& num) {
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
	element_ = ParseCardElement(element, nullptr);
	if (element_ == CardElement::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんな属性ないよ");
		return false;
	}
	return true; // Initialization successful
}

int CardCostCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Error: card is null
	}
	card->SetCost(ParseInt(num_, card));
	card->SetElementCost(element_);
	return 0;
}