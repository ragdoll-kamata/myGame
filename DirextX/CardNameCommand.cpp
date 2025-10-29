#include "CardNameCommand.h"
#include "ErrorMessage.h"

bool CardNameCommand::Initialize(const std::string& name) {
	if (name.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"名前ないよ");
		return false; // Initialization failed due to empty name
	}
	name_ = Utf8ToU32(name);
	return true;
}
ExecuteResult CardNameCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Execution failed due to null card
	}
	card->SetName(name_);
	return ExecuteResult::Normal; // Execution successful
}
