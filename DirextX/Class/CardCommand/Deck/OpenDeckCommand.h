#pragma once
#include "CardCommand.h"
class OpenDeckCommand : public CardCommand {
public:

	bool Initialize(const std::string& num, const std::string& card);

	// CardCommand を介して継承されました
	ExecuteResult Execute(Card* card) override;
private:
	std::string num_;
	std::string card_;
};

