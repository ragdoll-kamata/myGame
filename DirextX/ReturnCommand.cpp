#include "ReturnCommand.h"
#include "ErrorMessage.h"

bool ReturnCommand::Initialize(const std::vector<std::string>& commandTokens) {
	std::vector<std::string> commands;
	for (const std::string& str : commandTokens) {
		if (!Parse(str, commands)) {
			return false;
		}
	}
	commands.erase(commands.begin());

	if (commands.size() > 1) {
		// 1より多い場合、bool式として扱う
		parseBoolResult_ = ParseBool(commands);
		if (!parseBoolResult_ || parseBoolResult_->groups.size() == 0) {
			ErrorMessage::GetInstance()->SetMessage(U"bool式になってないよ");
			return false;
		}
	} else {
		if (commands.size() > 0) {
			value_ = commands[0];
		}
	}

	return true;
}

ExecuteResult ReturnCommand::Execute(Card* card) {
	if (parseBoolResult_) {
		if (!ExecuteBool(parseBoolResult_, card)) {
			card->SetReturnValue("false");
		} else {
			card->SetReturnValue("true");
		}
	} else {
		card->SetReturnValue(value_);
	}
	return ExecuteResult::Return;
}
