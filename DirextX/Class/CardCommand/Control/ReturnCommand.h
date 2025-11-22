#pragma once
#include "CardCommand.h"
class ReturnCommand : public CardCommand {
public:
	bool Initialize(const std::vector<std::string>& commandTokens);
	ExecuteResult Execute(Card* card) override;
private:
	std::string value_;
	std::unique_ptr<CardCommand::ParseBoolResult> parseBoolResult_;
};

