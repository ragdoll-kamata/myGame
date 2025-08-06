#include "CardTypeCommand.h"
#include "ErrorMessage.h"
bool CardTypeCommand::Initialize(const std::string& type) {
	if (type.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"タイプが空だよ");
		return false; // Invalid type
	}
	type_ = ParseCardType(type);
	if (type_ == CardType::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんなタイプはないよ");
		return false; // Parsing failed
	}
	return true;
}

int CardTypeCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Execution failed due to null card
	}
	card->SetType(type_);
	return 0; // Execution successful
}