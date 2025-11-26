#pragma once
#include "VoidCardCommand.h"
class ReturnCommand : public VoidCardCommand {
public:
	bool Initialize(const std::vector<std::string>& commandTokens);
	ExecuteResult Execute(Card* card) override;
private:
	std::string value_;
	std::unique_ptr<CardCommand::ParseBoolResult> parseBoolResult_;
};

