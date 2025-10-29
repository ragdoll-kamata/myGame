#pragma once
#include "CardCommand.h"
class AddHandCommand :
    public CardCommand {
public:
	bool Initialize(const std::string& card);

	ExecuteResult Execute(Card* card) override;
private:
	std::string card_;
};

