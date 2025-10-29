#pragma once
#include "CardCommand.h"
class CardExplanationCommand : public CardCommand {
public:
	bool Initialize(const std::string& explanation);

	ExecuteResult Execute(Card* card) override;
private:
	std::u32string explanation_;
};

