#include "LoadCard.h"

#include <iostream>
#include <fstream>
#include <sstream>



#include "CardCommandFactory.h"
#include "ErrorMessage.h"

void LoadCard::LoadCardFile(std::string filename) {
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
		std::string preToken;
		std::string key;
		TokenGroup commandTokens;
		commandTokens.lineNumber = lineNumber;
		for (const auto& token : parse) {
			if (token == "{") {
				if (!commandTokens.tokens.empty()) {
					if (commandTokens.type == TokenGroupType::Command) {
						ErrorMessage::GetInstance()->SetMessage(U"コマンドの後にネストは使えないよ");
						ErrorMessage::GetInstance()->SetErrorLine(lineNumber);
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
					ErrorMessage::GetInstance()->SetErrorLine(lineNumber);
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
					ErrorMessage::GetInstance()->SetErrorLine(lineNumber);
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

	for (int i = 0; i < tokenGroups.size(); ++i) {
		auto& group = tokenGroups[i];
		if (!tokenGroupFunctions[group.type](i)) {
			ErrorMessage::GetInstance()->SetErrorLine(tokenGroups[i].lineNumber);
			return; // エラーが発生した場合は処理を中断
		}
	}
}

std::vector<std::string> LoadCard::ParseLine(std::string& text) {
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

bool LoadCard::AdaptationCommand(int i) {
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

bool LoadCard::AdaptationIf(int i) {
	return false;
}

bool LoadCard::AdaptationElseIf(int i) {
	return false;
}

bool LoadCard::AdaptationElse(int i) {
	return false;
}

bool LoadCard::AdaptationWhile(int i) {
	return false;
}

bool LoadCard::AdaptationFor(int i) {
	return false;
}

bool LoadCard::AdaptationFunction(int i) {
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

bool LoadCard::AdaptationNestStart(int i) {
	nestStack.push(newNestID);
	newNestID++;
	return true;
}

bool LoadCard::AdaptationNestEnd(int i) {
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストの終わりが多いよ");
		return false;
	}
	nestStack.pop();
	return true;
}
