#pragma once
#include "CardCommand.h"
class CardNameCommand : public CardCommand {
public:
	bool Initialize(const std::string& name);

	int Execute(Card* card) override;
private:
	std::u32string name_;
};

