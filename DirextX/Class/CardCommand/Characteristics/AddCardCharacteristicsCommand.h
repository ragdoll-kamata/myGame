#pragma once
#include "CardCommand.h"
class AddCardCharacteristicsCommand : public CardCommand {
public:
	bool Initialize(const std::string& card, const std::string& characteristics);
	ExecuteResult Execute(Card* card) override;
private:
	std::string card_;
	CardCharacteristics characteristics_;
};

