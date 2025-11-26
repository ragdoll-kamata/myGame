#pragma once
#include "CardCommand.h"
class AddDeckBottomCommand : public CardCommand {
public:
	bool Initialize(std::string card);
	// CardCommand を介して継承されました
	ExecuteResult Execute(Card* card) override;
private:
	std::string card_;
};

