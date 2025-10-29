#include "IfCommand.h"
#include "CardData.h"
#include "ErrorMessage.h"

bool IfCommand::Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens) {
	commands_ = cardData->GetCardCommands(nestID);
	parseBoolResult_ = ParseBool(commandTokens);

	if (!parseBoolResult_ || parseBoolResult_->groups.size() == 0) {
		ErrorMessage::GetInstance()->SetMessage(U"if文成立してないよ");
		return false;
	}
	return true;
}

ExecuteResult IfCommand::Execute(Card* card) {
	if (ExecuteBool(parseBoolResult_, card)) {
		for (CardCommand* command : commands_) {
			ExecuteResult result = command->Execute(card);
			if (result != ExecuteResult::Normal) {
				return result;
			}
		}
	}
	return ExecuteResult::Normal;
}
