#include "IntVariableControlCommand.h"

bool IntVariableControlCommand::Initialize(std::string& intVariable, std::vector<std::string>& command) {
	if (!Parse(intVariable, commands_)) {
		return false;
	}
	for (const std::string& str : command) {
		if (!Parse(str, commands_)) {
			return false;
		}
	}

	intVariable_ = commands_[0];
	commands_.erase(commands_.begin());
	if (commands_[0] == "=") {
		type = CalculationType::Assign;
	} else if (commands_[0] == "+=") {
		type = CalculationType::Add;
	} else if (commands_[0] == "-=") {
		type = CalculationType::Subtract;
	} else if (commands_[0] == "*=") {
		type = CalculationType::Multiply;
	} else if (commands_[0] == "/=") {
		type = CalculationType::Division;
	} else {
		return false;
	}

	commands_.erase(commands_.begin());

	exprNode_ = CreateIntExprNode(commands_);
	
	return exprNode_ != nullptr;
}

ExecuteResult IntVariableControlCommand::Execute(Card* card) {
	int ver = card->GetInt(intVariable_);
	int value = CalculationIntExprNode(exprNode_, card);

	if (type == CalculationType::Assign) {
		ver = value;
	} else if (type == CalculationType::Add) {
		ver += value;
	} else if (type == CalculationType::Subtract) {
		ver -= value;
	} else if (type == CalculationType::Multiply) {
		ver *= value;
	} else if (type == CalculationType::Division) {
		if (value != 0) {
			ver /= value;
		}
	}
	card->SetInt(intVariable_, ver);

	return ExecuteResult::Normal;
}
