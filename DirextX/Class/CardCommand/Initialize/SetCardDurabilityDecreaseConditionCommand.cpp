#include "SetCardDurabilityDecreaseConditionCommand.h"
#include "ErrorMessage.h"

bool SetCardDurabilityDecreaseConditionCommand::Initialize(const std::string& conditions) {
	conditions_ = ParseBuildingDurabilityDecreasenTiming(conditions);
    if(conditions_ == BuildingDurabilityDecreasenTiming::Error) {
        ErrorMessage::GetInstance()->SetMessage(U"そんな耐久値減少条件はないよ");
        return false;
	}
    return true;
}

ExecuteResult SetCardDurabilityDecreaseConditionCommand::Execute(Card* card) {
	card->SetBuildingDurabilityDecreasenTiming(conditions_);
    return ExecuteResult::Normal;
}
