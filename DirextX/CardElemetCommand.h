#pragma once
#include "CardCommand.h"
class CardElemetCommand :
    public CardCommand {
public:
	bool Initialize(const std::string& element);

	ExecuteResult Execute(Card* card) override;
private:
	CardElement element_;
};

