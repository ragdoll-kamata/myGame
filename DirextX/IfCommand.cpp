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
	if (isExecuted_ || ExecuteBool(parseBoolResult_, card)) {
		isExecuted_ = true;
		int line = 1;
		for (CardCommand* command : commands_) {
			
			if (line < line_) {
				line++;
				continue;
			}
			ExecuteResult result = command->Execute(card);
			if(result == ExecuteResult::Standby) {
				line_ = line;
				return result;
			}
			if (result != ExecuteResult::Normal) {
				line_ = 0;
				return result;
			}
			line++;
		}
	}
	isExecuted_ = false;
	line_ = 0;
	return ExecuteResult::Normal;
}
