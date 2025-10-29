#include "CardTypeCommand.h"
#include "ErrorMessage.h"
bool CardTypeCommand::Initialize(const std::string& type) {
	if (type.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"タイプが空だよ");
		return false; // Invalid type
	}
	type_ = ParseCardType(type, nullptr);
	if (type_ == CardType::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんなタイプはないよ");
		return false; // Parsing failed
	}
	return true;
}

ExecuteResult CardTypeCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Execution failed due to null card
	}
	card->SetType(type_);
	return ExecuteResult::Normal; // Execution successful
}