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
int CardNameCommand::Execute(Card* card) {
		if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return -1; // Execution failed due to null card
	}
	card->SetName(name_);
	return 0; // Execution successful
}
