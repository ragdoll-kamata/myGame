#pragma once
#include "VoidCardCommand.h"
class SetCardDurabilityDecreaseConditionCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& conditions);
	ExecuteResult Execute(Card* card) override;
private:
	BuildingDurabilityDecreasenTiming conditions_;
};

