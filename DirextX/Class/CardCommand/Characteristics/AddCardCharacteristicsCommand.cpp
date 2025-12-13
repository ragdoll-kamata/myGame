#include "AddCardCharacteristicscommand.h"
#include "ErrorMessage.h"

bool AddCardCharacteristicscommand::Initialize(const std::string& card, const std::string& characteristics) {
	if(card.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false;
	}
	card_ = card;
	characteristics_ = ParseCardCharacteristics(characteristics);
	if (characteristics_ == CardCharacteristics::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんな特性はないよ");
		return false;
	}
	return true;
}

ExecuteResult AddCardCharacteristicscommand::Execute(Card* card) {
	std::vector<Card*> cards = card->GetCards(card_);
	for (auto& card : cards) {
		card->SetIsCardCharacteristics(characteristics_, true);
	}
	return ExecuteResult::Normal;
}
