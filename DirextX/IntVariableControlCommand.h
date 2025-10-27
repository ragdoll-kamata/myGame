#pragma once
#include "CardCommand.h"
enum class CalculationType {
	Add,
	Subtract,
	Multiply,
	Division,
	Assign,
};
class IntVariableControlCommand : public CardCommand {
public:

	bool Initialize(std::string intVariable, std::vector<std::string> command);

	int Execute(Card* card) override;

private:
	std::string intVariable_;
	std::vector<std::string> commands_;
	CalculationType type;
};

