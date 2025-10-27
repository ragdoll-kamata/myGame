#include "CardCommandFactory.h"

#include "CardNameCommand.h"
#include "CardTypeCommand.h"
#include "CardElemetCommand.h"
#include "CardExplanationCommand.h"
#include "CardCostCommand.h"

#include "OpenDeckCommand.h"

#include "AddHandCommand.h"

#include "AddCemeteryCommand.h"

#include "ElementFilterCommand.h"

#include "IfCommand.h"
#include "ReturnCommand.h"

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
	}
	///// 効果設定コマンド
	// 山札系
	if (key == "表向きにする") {
		return CreateOpenDeckCommand(commandTokens[0], commandTokens[1]);
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

	//　変数操作
	if (key.front() == '#') {

	}
	if (key.front() == '$') {

	}

	// keyの名前のコマンドが存在しない場合
	ErrorMessage::GetInstance()->SetMessage(U"そんなコマンドキーは存在しないよ");
	return std::unique_ptr<CardCommand>();
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardNameCommand(std::string cardName) {
	std::unique_ptr<CardNameCommand> cmd = std::make_unique<CardNameCommand>();
	if (cmd->Initialize(cardName)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardTypeCommand(std::string cardType) {
	std::unique_ptr<CardTypeCommand> cmd = std::make_unique<CardTypeCommand>();
	if (cmd->Initialize(cardType)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardElementCommand(std::string cardElement) {
	std::unique_ptr<CardElemetCommand> cmd = std::make_unique<CardElemetCommand>();
	if (cmd->Initialize(cardElement)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}

std::unique_ptr<CardCommand> CardCommandFactory::CreareCardExplanationCommand(std::string cardExplanation) {
	std::unique_ptr<CardExplanationCommand> cmd = std::make_unique<CardExplanationCommand>();
	if (cmd->Initialize(cardExplanation)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}
std::unique_ptr<CardCommand> CardCommandFactory::CreareCardCostCommand(std::string costElement, std::string costNum) {
	std::unique_ptr<CardCostCommand> cmd = std::make_unique<CardCostCommand>();
	if (cmd->Initialize(costElement, costNum)) {
		return cmd;
	}
	return std::unique_ptr<CardCommand>();
}


//////////////////////////////


// 山札系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateOpenDeckCommand(std::string num, std::string card) {
	std::unique_ptr<OpenDeckCommand> cmd = std::make_unique<OpenDeckCommand>();
	if (cmd->Initialize(num, card)) {
		return cmd;
	}

	return nullptr;
}

// 手札系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateAddHandCommand(std::string card) {
	std::unique_ptr<AddHandCommand> cmd = std::make_unique<AddHandCommand>();
	if (cmd->Initialize(card)) {
		return cmd;
	}

	return nullptr;
}

// 墓地系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateAddCemeteryCommand(std::string card) {
	std::unique_ptr<AddCemeteryCommand> cmd = std::make_unique<AddCemeteryCommand>();
	if (cmd->Initialize(card)) {
		return cmd;
	}

	return nullptr;
}

// カードフィルター系コマンドの生成
std::unique_ptr<CardCommand> CardCommandFactory::CreateElementFilterCommand(std::string element, std::string card, std::string card2) {
	std::unique_ptr<ElementFilterCommand> cmd = std::make_unique<ElementFilterCommand>();
	if (cmd->Initialize(element, card, card2)) {
		return cmd;
	}

	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateIfCommand(CardData* cardData, int nestID, std::vector<std::string> commandTokens) {
	std::unique_ptr<IfCommand> cmd = std::make_unique<IfCommand>();
	if (cmd->Initialize(cardData, nestID, commandTokens)) {
		return cmd;
	}
	return nullptr;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateReturnCommand() {
	std::unique_ptr<ReturnCommand> cmd = std::make_unique<ReturnCommand>();
	return cmd;
}

std::unique_ptr<CardCommand> CardCommandFactory::CreateNestMoveCommand(int index) {
	return std::unique_ptr<CardCommand>();
}

