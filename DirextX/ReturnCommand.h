#pragma once
#include "CardCommand.h"
class ReturnCommand : public CardCommand {
public:
	ExecuteResult Execute(Card* card) override;
};

