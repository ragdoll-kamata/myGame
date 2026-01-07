#pragma once
#include "VoidCardCommand.h"
class CardRarityCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& name);

	ExecuteResult Execute(Card* card) override;
private:
	CardRarity rarity_;
};

