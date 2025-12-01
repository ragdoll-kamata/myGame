#include "SetCardCharacteristicsCommand.h"
#include "ErrorMessage.h"


bool SetCardCharacteristicsCommand::Initialize(const std::vector<std::string>& characteristics) {
    for(std::string characteristicStr : characteristics) {
        CardCharacteristics characteristic = ParseCardCharacteristics(characteristicStr, nullptr);
        if (characteristic == CardCharacteristics::Error) {
            ErrorMessage::GetInstance()->SetMessage(U"そんな特性ないよ");
            return false;
        }
        characteristics_.push_back(characteristic);
	}
    return true;
}

ExecuteResult SetCardCharacteristicsCommand::Execute(Card* card) {
    if (card == nullptr) {
        ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
        return ExecuteResult::Error; // Error: card is null
	}
    for (CardCharacteristics characteristic : characteristics_) {
        card->SetIsCardCharacteristics(characteristic, true);
    }
    return ExecuteResult::Normal;
}
