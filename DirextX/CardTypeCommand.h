#pragma once
#include "CardCommand.h"
class CardTypeCommand :
    public CardCommand {
public:
	bool Initialize(const std::string& type);

	int Execute(Card* card) override;
private:
	CardType type_ = CardType::Error;
};

