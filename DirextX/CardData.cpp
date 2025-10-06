#include "CardData.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "CardCommandFactory.h"
#include "ErrorMessage.h"

void CardData::LoadCardFile(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return;
	}

	std::string line;
	int lineNumber = 0;
	while (std::getline(file, line)) {
		lineNumber++;
		std::vector<std::string> parse = ParseLine(line);
		CreateTokenGroup(parse, lineNumber);
	}

	for (int i = 0; i < tokenGroups.size(); ++i) {
		auto& group = tokenGroups[i];
		if (!tokenGroupFunctions[group.type](i)) {
			ErrorMessage::GetInstance()->SetErrorLine(tokenGroups[i].lineNumber);
			return; // エラーが発生した場合は処理を中断
		}
	}
}

bool CardData::CardFunctionLoad(Card* card, std::string functionName) {
	if (functionMap.contains(functionName)) {
		if (FunctionLoad(card, functionMap[functionName])) {
			return true;
		}
	}
	return false;
}

bool CardData::FunctionLoad(Card* card, int functionID) {
	if (cardCommands.contains(functionID)) {
		for (std::unique_ptr<CardCommand>& command : cardCommands[functionID]) {
			int i = command->Execute(card);
			if (i >= 0) {
				if (i > 0) {
					FunctionLoad(card, i);
				}
			} else {
				return false;
			}
		}
	}
	return true;
}

std::vector<std::string> CardData::ParseLine(std::string& text) {
	std::vector<std::string> tokens;
	std::string token;
	bool isSkip = false;
	for (char c : text) {
		if (c == '/') {
			if (!isSkip) {
				isSkip = true;
			} else {
				break;
			}
		} else {
			if (isSkip) {
				token.push_back('/');
			}
			isSkip = false;
		}

		if (isspace(static_cast<unsigned char>(c))) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else if (c == '{' || c == '}' || c == ':' || c == ',') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			tokens.push_back(std::string(1, c));
		} else if (c == '/') {

		} else {
			token.push_back(c);
		}
	}
	if (isSkip) {
		token.push_back('/');
	}
	if (!token.empty()) tokens.push_back(token);
	return tokens;
}

void CardData::CreateTokenGroup(std::vector<std::string>& tokens, int leneNum) {
	std::string preToken;
	TokenGroup commandTokens;
	commandTokens.lineNumber = leneNum;
	for (const auto& token : tokens) {
		if (token == "{") {
			if (!commandTokens.tokens.empty()) {
				if (commandTokens.type == TokenGroupType::Command) {
					ErrorMessage::GetInstance()->SetMessage(U"コマンドの後にネストは使えないよ");
					ErrorMessage::GetInstance()->SetErrorLine(leneNum);
					return;
				}
				tokenGroups.push_back(commandTokens);
				commandTokens.tokens.clear();
				commandTokens.type = TokenGroupType::None;
			}
			tokenGroups.push_back({{token}, TokenGroupType::NestStart});
			continue;

		}
		if (token == "}") {
			if (!commandTokens.tokens.empty()) {
				tokenGroups.push_back(commandTokens);
				commandTokens.tokens.clear();
				commandTokens.type = TokenGroupType::None;
			}
			tokenGroups.push_back({{token}, TokenGroupType::NestEnd});
			continue;
		}
		if (commandTokens.type == TokenGroupType::Command || commandTokens.type == TokenGroupType::If ||
			commandTokens.type == TokenGroupType::ElseIf || commandTokens.type == TokenGroupType::While ||
			commandTokens.type == TokenGroupType::For) {
			if (token == ",") continue; // コマンドの引数はカンマで区切る
			commandTokens.tokens.push_back(token);

		}
		if (token.front() == '@') {
			if (!commandTokens.tokens.empty()) {
				ErrorMessage::GetInstance()->SetMessage(U"関数定義前に何か書いてあるのはおかしいよ");
				ErrorMessage::GetInstance()->SetErrorLine(leneNum);
				return;
			}
			tokenGroups.push_back({{token}, TokenGroupType::Function});
			continue;
		}
		if (token == "if") {
			commandTokens.type = TokenGroupType::If;
			commandTokens.tokens.push_back(token);
			continue;
		}
		if (token == "else") {
			tokenGroups.push_back({{token}, TokenGroupType::Else});
			continue;
		}
		if (token == "elseif") {
			commandTokens.type = TokenGroupType::ElseIf;
			commandTokens.tokens.push_back(token);
			continue;
		}
		if (token == "while") {
			commandTokens.type = TokenGroupType::While;
			commandTokens.tokens.push_back(token);
			continue;
		}
		if (token == "for") {
			commandTokens.type = TokenGroupType::For;
			commandTokens.tokens.push_back(token);
			continue;
		}
		if (token == ":") {
			if (commandTokens.type == TokenGroupType::Command) {
				ErrorMessage::GetInstance()->SetMessage(U"同じ行にコマンドは1個までだよ");
				ErrorMessage::GetInstance()->SetErrorLine(leneNum);
				return;
			}
			commandTokens.tokens.push_back(preToken);
			commandTokens.tokens.push_back(token);
			commandTokens.type = TokenGroupType::Command;
		}
		preToken = token;
	}
	if (!commandTokens.tokens.empty() && commandTokens.type != TokenGroupType::None) {
		tokenGroups.push_back(commandTokens);
	}
}

bool CardData::AdaptationCommand(int i) {
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストに入ってないよ");
		return false;
	}
	std::string pre;
	std::string key;
	std::vector<std::string> commandTokens;
	bool isCommand = false;
	for (const auto& token : tokenGroups[i].tokens) {
		if (isCommand) {
			commandTokens.push_back(token);
		}
		if (token == ":") {
			key = pre;
			isCommand = true;
			continue;
		}

		pre = token;
	}
	std::unique_ptr<CardCommand> command = CardCommandFactory::CreateCommand(key, commandTokens);
	if (command == nullptr) {
		//ErrorMessage::GetInstance()->SetMessage(U"コマンドが見つからないよ");
		return false;
	}

	cardCommands[nestStack.top()].push_back(std::move(command));

	return true;
}

bool CardData::AdaptationIf(int i) {
	return false;
}

bool CardData::AdaptationElseIf(int i) {
	return false;
}

bool CardData::AdaptationElse(int i) {
	return false;
}

bool CardData::AdaptationWhile(int i) {
	return false;
}

bool CardData::AdaptationFor(int i) {
	return false;
}

bool CardData::AdaptationFunction(int i) {
	if (tokenGroups[i + 1].type != TokenGroupType::NestStart) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストが見つからないよ");
		return false;
	}
	if(!nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネスト内で関数定義はできないよ");
		return false;
	}
	std::string functionName = tokenGroups[i].tokens[0].substr(1); // '@'を除去

	functionMap[functionName] = newNestID;

	return true;
}

bool CardData::AdaptationNestStart(int i) {
	nestStack.push(newNestID);
	newNestID++;
	return true;
}

bool CardData::AdaptationNestEnd(int i) {
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストの終わりが多いよ");
		return false;
	}
	nestStack.pop();
	return true;
}
