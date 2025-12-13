#include "CardCommandFactory.h"

#include "Initialize/CardNameCommand.h"
#include "Initialize/CardTypeCommand.h"
#include "Initialize/CardElemetCommand.h"
#include "Initialize/CardExplanationCommand.h"
#include "Initialize/CardCostCommand.h"
#include "Initialize/SetCardCharacteristicsCommand.h"
#include "Initialize/SetCardDurabilityCommand.h"
#include "Initialize/SetCardDurabilityDecreaseConditionCommand.h"
#include "Initialize/SetCardActivationTimingCommand.h"

#include "Deck/OpenDeckCommand.h"
#include "Deck/SelectOpenDeckCommand.h"
#include "Deck/AddDeckBottomCommand.h"

#include "Hand/AddHandCommand.h"
#include "Hand/SelectHandCommand.h"

#include "Cemetery/AddCemeteryCommand.h"

#include "Filter/ElementFilterCommand.h"

#include "Characteristics/AddCardCharacteristicscommand.h"

#include "Variable/IntVariableControlCommand.h"
#include "Variable/CardVariableControlCommand.h"

#include "Control/IfCommand.h"
#include "Control/ReturnCommand.h"

#include "ErrorMessage.h"

std::unique_ptr<CardCommand> CardCommandFactory::CreateCommand(std::string key, std::vector<std::string>& commandTokens) {
	
	// 初期設定コマンド
	if (key == "カード名") {
		return CreareCardNameCommand(commandTokens[0]);
	} else if (key == "カードタイプ") {
		return CreareCardTypeCommand(commandTokens[0]);
	} else if (key == "属性") {
		return CreareCardElementCommand(commandTokens[0]);
	} else if (key == "カード説明") {
		return CreareCardExplanationCommand(commandTokens[0]);
	} else if (key == "エネルギーコスト") {
		return CreareCardCostCommand(commandTokens[0], commandTokens[1]);
	} else if (key == "画像") {
		//return CreareCardNameCommand(commandTokens[0]);
	} else if (key == "カード特性") {
		return CreareSetCardCharacteristicsCommand(commandTokens);
	} else if (key == "耐久値") {
		return CreareSetCardDurabilityCommand(commandTokens[0]);
	} else if (key == "耐久値減少条件") {
		return CreareSetCardDurabilityDecreaseConditionCommand(commandTokens[0]);
	} else if (key == "発動タイミング") {
		return CreareSetCardActivationTimingCommand(commandTokens[0]);
	}

	///// 効果設定コマンド
	// 山札系
	if (key == "表向きにする") {
		return CreateOpenDeckCommand(commandTokens[0], commandTokens[1]);
	} else if (key == "表向き選択") {
		return CreateSelectOpenDeckCommand(commandTokens[0], commandTokens[1], commandTokens[2], commandTokens[3], commandTokens[4]);
	} else if (key == "山札下送り") {
		return CreateAddDeckBottomCommand(commandTokens[0]);
	}

	// 手札系
	if (key == "手札に加える") {
		return CreateAddHandCommand(commandTokens[0]);
	}

	// 墓地系
	if (key == "墓地送り") {
		return CreateAddCemeteryCommand(commandTokens[0]);
	}

	// カードフィルター系
	if (key == "属性フィルター") {
		return CreateElementFilterCommand(commandTokens[0], commandTokens[1], commandTokens[2]);
	}

	// 特性
	if (key == "特性付与") {

	}

	//　変数操作
	if (key.front() == '#') {
		return CreateIntVariableControlCommand(key, commandTokens);
	}else if (key.front() == '$') {
		return CreateCardVariableControlCommand(key, commandTokens);
	}

	// keyの名前のコマンドが存在しない場合
	ErrorMessage::GetInstance()->SetMessage(U"そんなコマンドキーは存在しないよ");
	return nullptr;
}

// 初期設定コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreareCardNameCommand(std::string& cardName) {
	std::unique_ptr<CardNameCommand> cmd = std::make_unique<CardNameCommand>();
	if (cmd->Initialize(cardName)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardTypeCommand(std::string& cardType) {
	std::unique_ptr<CardTypeCommand> cmd = std::make_unique<CardTypeCommand>();
	if (cmd->Initialize(cardType)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardElementCommand(std::string& cardElement) {
	std::unique_ptr<CardElemetCommand> cmd = std::make_unique<CardElemetCommand>();
	if (cmd->Initialize(cardElement)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardExplanationCommand(std::string& cardExplanation) {
	std::unique_ptr<CardExplanationCommand> cmd = std::make_unique<CardExplanationCommand>();
	if (cmd->Initialize(cardExplanation)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}
std::unique_ptr<CardCommand> CardCommandFactory::CreareCardCostCommand(std::string& costElement, std::string& costNum) {
	std::unique_ptr<CardCostCommand> cmd = std::make_unique<CardCostCommand>();
	if (cmd->Initialize(costElement, costNum)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareSetCardCharacteristicsCommand(std::vector<std::string>& characteristics) {
	std::unique_ptr<SetCardCharacteristicsCommand> cmd = std::make_unique<SetCardCharacteristicsCommand>();
	if (cmd->Initialize(characteristics)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareSetCardDurabilityCommand(std::string& durability) {
	std::unique_ptr<SetCardDurabilityCommand> cmd = std::make_unique<SetCardDurabilityCommand>();
	if (cmd->Initialize(durability)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareSetCardActivationTimingCommand(std::string& activationTimings) {
	std::unique_ptr<SetCardActivationTimingCommand> cmd = std::make_unique<SetCardActivationTimingCommand>();
	if (cmd->Initialize(activationTimings)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareSetCardDurabilityDecreaseConditionCommand(std::string& conditions) {
	std::unique_ptr<SetCardDurabilityDecreaseConditionCommand> cmd = std::make_unique<SetCardDurabilityDecreaseConditionCommand>();
	if (cmd->Initialize(conditions)) {
		return cmd;
	}
	return nullptr;
}

//////////////////////////////


// 山札系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateOpenDeckCommand(std::string& num, std::string& card) {
	std::unique_ptr<OpenDeckCommand> cmd = std::make_unique<OpenDeckCommand>();
	if (cmd->Initialize(num, card)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateSelectOpenDeckCommand(std::string& num, std::string& minSelect, std::string& maxSelect, std::string& selectCard, std::string& notSelectCard) {
	std::unique_ptr<SelectOpenDeckCommand> cmd = std::make_unique<SelectOpenDeckCommand>();
	if (cmd->Initialize(num, minSelect, maxSelect, selectCard, notSelectCard)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateAddDeckBottomCommand(std::string& card) {
	std::unique_ptr<AddDeckBottomCommand> cmd = std::make_unique<AddDeckBottomCommand>();
	if (cmd->Initialize(card)) {
		return cmd;
	}
	return nullptr;
}

// 手札系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateAddHandCommand(std::string& card) {
	std::unique_ptr<AddHandCommand> cmd = std::make_unique<AddHandCommand>();
	if (cmd->Initialize(card)) {
		return cmd;
	}
	return nullptr;
}

// 墓地系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateAddCemeteryCommand(std::string& card) {
	std::unique_ptr<AddCemeteryCommand> cmd = std::make_unique<AddCemeteryCommand>();
	if (cmd->Initialize(card)) {
		return cmd;
	}

	return nullptr;
}

// カードフィルター系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateElementFilterCommand(std::string& element, std::string& card, std::string& card2) {
	std::unique_ptr<ElementFilterCommand> cmd = std::make_unique<ElementFilterCommand>();
	if (cmd->Initialize(element, card, card2)) {
		return cmd;
	}

	return nullptr;
}

// 変数操作系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateIntVariableControlCommand(std::string& intVariable, std::vector<std::string>& command) {
	std::unique_ptr<IntVariableControlCommand> cmd = std::make_unique<IntVariableControlCommand>();
	if (cmd->Initialize(intVariable, command)) {
		return cmd;
	}

	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateCardVariableControlCommand(std::string& cardVariable, std::vector<std::string>& command) {
	std::unique_ptr<CardVariableControlCommand> cmd = std::make_unique<CardVariableControlCommand>();
	if (cmd->Initialize(cardVariable, command)) {
		return cmd;
	}

	return nullptr;
}

// コマンド系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateIfCommand(CardData* cardData, int nestID, const std::vector<std::string>& commandTokens) {
	std::unique_ptr<IfCommand> cmd = std::make_unique<IfCommand>();
	if (cmd->Initialize(cardData, nestID, commandTokens)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateReturnCommand(std::vector<std::string>& commandTokens) {
	std::unique_ptr<ReturnCommand> cmd = std::make_unique<ReturnCommand>();
	if(cmd->Initialize(commandTokens)) {
		return cmd;
	}
	return nullptr;
}