#pragma once
#include<memory>
#include "CardCommand.h"

class CardData;
namespace CardCommandFactory
{
	std::unique_ptr<CardCommand> CreateCommand(std::string key, std::vector<std::string>& commandTokens);

	std::unique_ptr<CardCommand> CreareCardNameCommand(std::string cardName);
	std::unique_ptr<CardCommand> CreareCardTypeCommand(std::string cardType);
	std::unique_ptr<CardCommand> CreareCardElementCommand(std::string cardElement);
	std::unique_ptr<CardCommand> CreareCardExplanationCommand(std::string cardExplanation);
	std::unique_ptr<CardCommand> CreareCardCostCommand(std::string costElement,std::string costNum);

	

	std::unique_ptr<CardCommand> CreateOpenDeckCommand(std::string num, std::string card);

	std::unique_ptr<CardCommand> CreateAddHandCommand(std::string card);

	std::unique_ptr<CardCommand> CreateAddCemeteryCommand(std::string card);

	std::unique_ptr<CardCommand> CreateElementFilterCommand(std::string element, std::string card, std::string card2);


	std::unique_ptr<CardCommand> CreateIfCommand(CardData* cardData, int nestID, std::vector<std::string> commandTokens);

	std::unique_ptr<CardCommand> CreateReturnCommand();

	std::unique_ptr<CardCommand> CreateNestMoveCommand(int index);
}