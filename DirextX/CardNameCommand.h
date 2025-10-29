#pragma once
#include "CardCommand.h"
class CardNameCommand : public CardCommand {
public:
	bool Initialize(const std::string& name);

	ExecuteResult Execute(Card* card) override;
private:
	std::u32string name_;
};

