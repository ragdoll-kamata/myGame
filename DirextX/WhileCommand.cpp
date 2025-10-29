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

ExecuteResult WhileCommand::Execute(Card* card) {
	bool isBreak = false;
	while (ExecuteBool(parseBoolResult_, card)) {
		for (CardCommand* command : commands_) {
			ExecuteResult result = command->Execute(card);
			if (result == ExecuteResult::Break) {
				return ExecuteResult::Normal;
			}
			if (result == ExecuteResult::Continue) {
				break;
			}
			if (result != ExecuteResult::Normal) {
				return result;
			}
		}
	}
	return ExecuteResult::Normal;
}