#pragma once
#include "CardCommand.h"
enum class CalculationType {
	Add,
	Subtract,
	Multiply,
	Division,
	Assign,
	None,
};
class IntVariableControlCommand : public CardCommand {
public:

	bool Initialize(std::string& intVariable, std::vector<std::string>& command);

	ExecuteResult Execute(Card* card) override;

private:
	std::string intVariable_;
	std::vector<std::string> commands_;
	CalculationType type;
	std::unique_ptr<IntExprNode> exprNode_;
};

