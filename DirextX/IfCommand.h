#pragma once
#include "CardCommand.h"
class CardData;
class IfCommand : public CardCommand {
public:
	bool Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens);

	int Execute(Card* card) override;

private:
	std::vector<CardCommand*> commands_;
	CardCommand::ParseBoolResult parseBoolResult_;
};

