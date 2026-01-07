#include "CardRarityCommand.h"
#include "ErrorMessage.h"

bool CardRarityCommand::Initialize(const std::string& name) {
	rarity_ = ParseCardRarity(name);
	if (rarity_ == CardRarity::Error) {
		ErrorMessage::GetInstance()->SetMessage(U"そんなレアリティないよ");
		return false;
	}
	return true;
}

ExecuteResult CardRarityCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error;
	}
	card->SetRarity(rarity_);
	return ExecuteResult::Normal;
}
