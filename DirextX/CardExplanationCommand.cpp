#include "CardExplanationCommand.h"
#include "ErrorMessage.h"

bool CardExplanationCommand::Initialize(const std::string& explanation) {
	if (explanation.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"名前ないよ");
		return false;
	}
	explanation_ = Utf8ToU32(explanation);
	return true;
}

ExecuteResult CardExplanationCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error;
	}
	card->SetDescription(explanation_);
	return ExecuteResult::Normal;
}