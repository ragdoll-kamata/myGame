#pragma once
#include "CardCommand.h"
class AddMoneyCommand : public CardCommand {
public:
	bool Initialize(const std::string& money);

	ExecuteResult Execute(Card* card) override;
private:
	int money_;
};

