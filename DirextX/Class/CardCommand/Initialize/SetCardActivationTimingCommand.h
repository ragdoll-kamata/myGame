#pragma once
#include "VoidCardCommand.h"
class SetCardActivationTimingCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& activationTimings);
	ExecuteResult Execute(Card* card) override;
private:
	BuildingActivationTiming activationTimings_;
};

