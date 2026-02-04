#include "SetSelectCardIfCommand.h"
#include "ErrorMessage.h"

bool SetSelectCardIfCommand::Initialize(const std::string& functionName) {
	functionName_ = functionName;
	return true;
}

ExecuteResult SetSelectCardIfCommand::Execute(Card* card) {
	if (card->HasFunction(functionName_)) {
		card->SetSelectCardIfFunctionName(functionName_);
		return ExecuteResult::Normal;
	}
	ErrorMessage::GetInstance()->SetMessage(U"そんな関数はないよ");
	return ExecuteResult::Error;
}
