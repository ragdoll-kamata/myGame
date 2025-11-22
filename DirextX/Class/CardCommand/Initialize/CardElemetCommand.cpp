#include "CardElemetCommand.h"
#include "ErrorMessage.h"

bool CardElemetCommand::Initialize(const std::string& element) {
	if (element.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"名前ないよ");
		return false; // Initialization failed due to empty name
	}
	element_ = ParseCardElement(element, nullptr);
	if (element_ == CardElement::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんな属性ないよ");
		return false;
	}
	return true;
}

ExecuteResult CardElemetCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Execution failed due to null card
	}
	card->SetElement(element_);
	return ExecuteResult::Normal; // Execution successful
}
