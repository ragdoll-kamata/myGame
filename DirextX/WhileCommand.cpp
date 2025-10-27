#include "WhileCommand.h"
#include "CardData.h"
#include "ErrorMessage.h"

bool WhileCommand::Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens) {
	commands_ = cardData->GetCardCommands(nestID);
	parseBoolResult_ = ParseBool(commandTokens);

	if (!parseBoolResult_ || parseBoolResult_->groups.size() == 0) {
		ErrorMessage::GetInstance()->SetMessage(U"while文成立してないよ");
		return false;
	}
	return true;
}

int WhileCommand::Execute(Card* card) {
	bool isBreak = false;
	while (ExecuteBool(parseBoolResult_, card)) {
		for (CardCommand* command : commands_) {
			int result = command->Execute(card);
			if (result == -4) {
				return 0;
			}
			if (result == -5) {
				break;
			}
			if (result != 0) {
				return result;
			}
		}
	}
	return 0;
}