#pragma once
#include "CardCommand.h"
class AddCemeteryCommand :
    public CardCommand {
public:
	bool Initialize(const std::string& card);

	int Execute(Card* card) override;
private:
	std::string card_;
};

