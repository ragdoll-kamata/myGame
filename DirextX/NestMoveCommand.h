#pragma once
#include "CardCommand.h"
class NestMoveCommand : public CardCommand {
public:

	bool Initialize(int index);

	// CardCommand を介して継承されました
	int Execute(Card* card) override;
private:
	int index_ = -1;
};

