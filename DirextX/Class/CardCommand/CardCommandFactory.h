#pragma once
#include<memory>
#include "CardCommand.h"
#include <functional>

class CardData;
namespace CardCommandFactory
{
	std::unique_ptr<CardCommand> CreateCommand(std::string key, std::vector<std::string>& commandTokens);

	// 
	std::unique_ptr<CardCommand>  CreateCommandIfArgsValid(int size, int index, std::function< std::unique_ptr<CardCommand>()> func);

	// 初期設定コマンドの生成
	std::unique_ptr<CardCommand> CreareCardNameCommand(std::string& cardName);
	std::unique_ptr<CardCommand> CreareCardTypeCommand(std::string& cardType);
	std::unique_ptr<CardCommand> CreareCardElementCommand(std::string& cardElement);
	std::unique_ptr<CardCommand> CreareCardExplanationCommand(std::string& cardExplanation);
	std::unique_ptr<CardCommand> CreareCardCostCommand(std::string& costElement,std::string& costNum);
	std::unique_ptr<CardCommand> CreareCardRarityCommand(std::string& cardRarity);
	std::unique_ptr<CardCommand> CreareSetCardCharacteristicsCommand(std::vector<std::string>& characteristics);
	std::unique_ptr<CardCommand> CreareSetCardDurabilityCommand(std::string& durability);
	std::unique_ptr<CardCommand> CreareSetCardActivationTimingCommand(std::string& activationTimings);
	std::unique_ptr<CardCommand> CreareSetCardDurabilityDecreaseConditionCommand(std::string& conditions);

	// リソース系コマンドの生成
	std::unique_ptr<CardCommand> CreateAddMoneyCommand(std::string& num);

	// 山札系コマンドの生成
	std::unique_ptr<CardCommand> CreateOpenDeckCommand(std::string& num, std::string& card);
	std::unique_ptr<CardCommand> CreateSelectOpenDeckCommand(std::string& num, std::string& minSelect, std::string& maxSelect, std::string& selectCard, std::string& notSelectCard);
	std::unique_ptr<CardCommand> CreateAddDeckBottomCommand(std::string& card);

	// 手札系コマンドの生成
	std::unique_ptr<CardCommand> CreateAddHandCommand(std::string& card);
	std::unique_ptr<CardCommand> CreateSelectHandCommand(std::string& minSelect, std::string& maxSelect, std::string& selectCard, std::string& notSelectCard);

	// 墓地系コマンドの生成
	std::unique_ptr<CardCommand> CreateAddCemeteryCommand(std::string& card);

	// カードフィルター系コマンドの生成
	std::unique_ptr<CardCommand> CreateElementFilterCommand(std::string& element, std::string& card, std::string& card2);

	// 特性付与コマンドの生成
	std::unique_ptr<CardCommand> CreateAddCardCharacteristicsCommand(std::string& card, std::string& characteristics);

	// 選択条件設定コマンドの生成
	std::unique_ptr<CardCommand> CreateSetSelectCardIfCommandCommand(std::string& functionName);

	// 制御系コマンドの生成
	std::unique_ptr<CardCommand> CreateIfCommand(CardData* cardData, int nestID, const std::vector<std::string>& commandTokens);
	std::unique_ptr<CardCommand> CreateReturnCommand(std::vector<std::string>& commandTokens);

	// 変数操作コマンドの生成
	std::unique_ptr<CardCommand> CreateIntVariableControlCommand(std::string& intVariable, std::vector<std::string>& command);
	std::unique_ptr<CardCommand> CreateCardVariableControlCommand(std::string& cardVariable, std::vector<std::string>& command);
}