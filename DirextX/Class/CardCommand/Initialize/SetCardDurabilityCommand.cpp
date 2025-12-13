#include "SetCardDurabilityCommand.h"
#include "ErrorMessage.h"


bool SetCardDurabilityCommand::Initialize(const std::string& durability) {
    if(durability.front() != '#') {
		std::unique_ptr<Card> cardptr = std::make_unique<Card>();
		int number = ParseInt(durability, cardptr.get());
		if (number < 0) {
			ErrorMessage::GetInstance()->SetMessage(U"数字に変換できないよ");
			return false;
		}
	}
	durability_ = durability;
    return true;
}

ExecuteResult SetCardDurabilityCommand::Execute(Card* card) {
	int durability = ParseInt(durability_, card);
	card->SetDurability(durability);
    return ExecuteResult::Normal;
}
