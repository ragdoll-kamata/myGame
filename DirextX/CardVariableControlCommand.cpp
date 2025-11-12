#include "CardVariableControlCommand.h"

bool CardVariableControlCommand::Initialize(std::string& cardVariable, std::vector<std::string>& command) {
	if (!Parse(cardVariable, commands_)) {
		return false;
	}
	for (const std::string& str : command) {
		if (!Parse(str, commands_)) {
			return false;
		}
	}
	cardVariable_ = commands_[0];
	commands_.erase(commands_.begin());
	if (commands_[0] == "=") {
		type = VariableCalculationType::Assign;
	} else if (commands_[0] == "+=") {
		type = VariableCalculationType::Add;
	} else if (commands_[0] == "-=") {
		type = VariableCalculationType::Subtract;
	} else {
		return false;
	}

	commands_.erase(commands_.begin());
	exprNode_ = CreateCardExprNode(commands_);

	return exprNode_ != nullptr;
}

ExecuteResult CardVariableControlCommand::Execute(Card* card) {
	std::vector<Card*> cards = card->GetCards(cardVariable_);
	std::vector<Card*> cards2;
	if (!CalculationCardExprNode(exprNode_, cards2, card)) {
		return ExecuteResult::Error;
	}
	if (type == VariableCalculationType::Assign) {
		cards = cards2;
	} else if (type == VariableCalculationType::Add) {
		cards.insert(cards.end(), cards2.begin(), cards2.end());
	} else if (type == VariableCalculationType::Subtract) {
		for (Card* c : cards2) {
			auto it = std::find(cards.begin(), cards.end(), c);
			if (it != cards.end()) {
				cards.erase(it);
			}
		}
	}
	card->SetCards(cardVariable_, cards);

	return ExecuteResult::Normal;
}