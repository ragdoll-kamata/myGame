#pragma once
#include "CardCommand.h"
class ReturnCommand : public CardCommand {
public:
	int Execute(Card* card) override;
};

