#pragma once
#include "VoidCardCommand.h"
class SetCardDurabilityCommand : public VoidCardCommand {
public:
		bool Initialize(const std::string& durability);
		ExecuteResult Execute(Card* card) override;
private:
	std::string durability_;
};

