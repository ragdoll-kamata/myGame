#pragma once
#include "CardCommand.h"
class OpenDeckCommand : public CardCommand {
public:

	bool Initialize(std::string num, std::string card);

	// CardCommand を介して継承されました
	int Execute(Card* card) override;
private:
	std::string num_;
	std::string card_;
};

