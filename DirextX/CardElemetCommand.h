#pragma once
#include "CardCommand.h"
class CardElemetCommand :
    public CardCommand {
public:
	bool Initialize(const std::string& element);

	int Execute(Card* card) override;
private:
	CardElement element_;
};

