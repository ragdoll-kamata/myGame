#include "CardExplanationCommand.h"
#include "ErrorMessage.h"

bool CardExplanationCommand::Initialize(const std::string& explanation) {
	if (explanation.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"名前ないよ");
		return false;
	}
	explanation_ = Utf8ToU32(explanation);
	bool is = false;
	std::u32string str = explanation_;
	for(auto & c : str){
		if(c == U'\\'){
			is = true;
			continue;
		}
		if(is){
			if(c == U'n'){
				explanation_.replace(explanation_.find(U"\\n"), 2, U"\n");
			}
			is = false;
		} else {
			is = false;
		}
	}
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