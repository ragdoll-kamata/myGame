#pragma once
#include "VoidCardCommand.h"
class ElementFilterCommand : public VoidCardCommand {
public:
	bool Initialize(std::string enement,std::string card, std::string FilterCard);
	ExecuteResult Execute(Card* card) override;
private:
	std::string filterCard_;
	std::string card_;
	CardElement element_;
	bool isCardValue_ = false;
};

