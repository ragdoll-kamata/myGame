#pragma once
#include "CardCommand.h"
class CardData;
class IfCommand : public CardCommand {
public:
	bool Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens);
private:
	std::vector<CardCommand*> commands_;
};

