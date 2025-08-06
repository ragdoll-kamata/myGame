#include "CardExplanationCommand.h"
#include "ErrorMessage.h"

bool CardExplanationCommand::Initialize(const std::string& explanation) {
	if (explanation.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"名前ないよ");
		return false; // Initialization failed due to empty name
	}
	explanation_ = Utf8ToU32(explanation);
	return true;
}

int CardExplanationCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Execution failed due to null card
	}
	card->SetDescription(explanation_);
	return 0; // Execution successful
}
