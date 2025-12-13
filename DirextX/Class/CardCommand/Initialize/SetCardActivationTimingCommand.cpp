#include "SetCardActivationTimingCommand.h"
#include "ErrorMessage.h"

bool SetCardActivationTimingCommand::Initialize(const std::string& activationTimings) {
	activationTimings_ = ParseBuildingActivationTiming(activationTimings);
	if (activationTimings_ == BuildingActivationTiming::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんな建物の発動タイミングはないよ");
		return false;
	}
	return true;
}

ExecuteResult SetCardActivationTimingCommand::Execute(Card* card) {
	card->SetBuildingActivationTiming(activationTimings_);
	return ExecuteResult::Normal;
}
