#pragma once
#include "CardCommand.h"
enum class VariableCalculationType {
	Add,
	Subtract,
	Multiply,
	Division,
	Assign,
	None,
};
class CardVariableControlCommand : public CardCommand {
public:
	bool Initialize(std::string& cardVariable, std::vector<std::string>& command);
	ExecuteResult Execute(Card* card) override;
private:
	std::string cardVariable_;
	std::vector<std::string> commands_;
	VariableCalculationType type;
	std::unique_ptr<ExprNode> exprNode_;
};

