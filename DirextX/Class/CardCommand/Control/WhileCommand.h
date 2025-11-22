#pragma once
#include "CardCommand.h"
class CardData;
class WhileCommand : public CardCommand {
public:
	bool Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens);

	ExecuteResult Execute(Card* card) override;

private:
	std::vector<CardCommand*> commands_;
	std::unique_ptr<CardCommand::ParseBoolResult> parseBoolResult_;
};

