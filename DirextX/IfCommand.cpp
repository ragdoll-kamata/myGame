#include "IfCommand.h"
#include "CardData.h"

bool IfCommand::Initialize(CardData* cardData, int nestID, std::vector<std::string> commandTokens) {
	commands_ = cardData->GetCardCommands(nestID);

	return false;
}
