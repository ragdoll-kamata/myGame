#pragma once
#include "CardCommand.h"
class CardCostCommand : public CardCommand {
public:
	bool Initialize(const std::string& element, const std::string& num);

	ExecuteResult Execute(Card* card) override;
private:
	CardElement element_;
	std::string num_;
};

