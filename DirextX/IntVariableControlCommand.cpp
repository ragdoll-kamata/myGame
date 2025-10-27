#include "IntVariableControlCommand.h"

bool IntVariableControlCommand::Initialize(std::string intVariable, std::vector<std::string> command) {
	intVariable_ = intVariable;
	if (command[0] == "=") {

	} else if (command[0] == "+=") {

	} else if (command[0] == "-=") {

	} else if (command[0] == "*=") {

	} else if (command[0] == "/=") {

	} else {
		return false;
	}




	commands_ = command;
	return true;
}

int IntVariableControlCommand::Execute(Card* card) {
	int i = 0;

	for (auto& command : commands_) {

	}
	return 0;
}
