#include "IntVariableControlCommand.h"

bool IntVariableControlCommand::Initialize(std::string intVariable, std::vector<std::string> command) {
	if (command.size() <= 0) {
		return false;
	}
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



	return true;
}

ExecuteResult IntVariableControlCommand::Execute(Card* card) {

	return ExecuteResult::Normal;
}
