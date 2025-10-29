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
		lineNum = tokenGroups[i].lineNumber;
		// トークングループのタイプに対応する関数を呼び出す
		if (!tokenGroupFunctions[group.type](i)) {
			ErrorMessage::GetInstance()->SetErrorLine(tokenGroups[i].lineNumber);
			return; // エラーが発生した場合は処理を中断
		}
	}
	while (!commandQueue.empty()) {
		commandQueue.front()();
		commandQueue.pop();
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

std::vector<CardCommand*> CardData::GetCardCommands(int functionID) {
	std::vector<CardCommand*> commands;
	if (cardCommands.contains(functionID)) {
		for (const auto& command : cardCommands[functionID]) {
			commands.push_back(command.get());
		}
	}
	return commands;
}

bool CardData::FunctionLoad(Card* card, int functionID) {
	if (cardCommands.contains(functionID)) {
		for (std::unique_ptr<CardCommand>& command : cardCommands[functionID]) {
			ExecuteResult i = command->Execute(card);
			if (i == ExecuteResult::Return) {
				break;
			}
			if (i == ExecuteResult::Error) {
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
		// コメント処理
		if (c == '/') {
			if (!isSkip) {
				// 最初の/ならスキップフラグを立てる
				isSkip = true;
			} else {
				// 2つ目の/ならコメント開始なのでループを抜ける
				break;
			}
		} else {
			// /以外の文字が来たらスキップフラグをリセットして、/をトークンに追加
			if (isSkip) {
				token.push_back('/');
			}
			isSkip = false;
		}

		// 空白文字はスキップ
		if (isspace(static_cast<unsigned char>(c))) {
			continue;
		} else if (c == '{' || c == '}' || c == ':' || c == ',' || c == '(' || c == ')') {
			// これらの文字はそれ自体がトークン
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			tokens.push_back(std::string(1, c));
		} else if (c == '/') {
			// /は処理済み
		} else {
			// その他の文字はトークンに追加
			token.push_back(c);
		}
	}

	if (isSkip) {
		// 行の最後が/で終わっていた場合、その/をトークンに追加
		token.push_back('/');
	}
	// 最後のトークンを追加
	if (!token.empty()) tokens.push_back(token);
	return tokens;
}

void CardData::CreateTokenGroup(std::vector<std::string>& tokens, int leneNum) {
	std::string preToken;
	TokenGroup commandTokens;
	commandTokens.lineNumber = leneNum;
	for (const auto& token : tokens) {
		// ネストの開始
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
		// ネストの終了
		if (token == "}") {
			if (!commandTokens.tokens.empty()) {
				tokenGroups.push_back(commandTokens);
				commandTokens.tokens.clear();
				commandTokens.type = TokenGroupType::None;
			}
			tokenGroups.push_back({{token}, TokenGroupType::NestEnd});
			continue;
		}
		if (token == "return") {
			if(!commandTokens.tokens.empty()) {
				ErrorMessage::GetInstance()->SetMessage(U"returnの前に何か書いてあるのはおかしいよ");
				ErrorMessage::GetInstance()->SetErrorLine(leneNum);
				return;
			}
			tokenGroups.push_back({{token}, TokenGroupType::Return});
			continue;
		}
		// コマンドの引数
		if (commandTokens.type == TokenGroupType::Command || commandTokens.type == TokenGroupType::If ||
			commandTokens.type == TokenGroupType::ElseIf || commandTokens.type == TokenGroupType::While ||
			commandTokens.type == TokenGroupType::For) {
			if (token == ",") continue; // コマンドの引数はカンマで区切る
			commandTokens.tokens.push_back(token);
			continue;
		}
		// 関数定義
		if (token.front() == '@') {
			if (!commandTokens.tokens.empty()) {
				ErrorMessage::GetInstance()->SetMessage(U"関数定義前に何か書いてあるのはおかしいよ");
				ErrorMessage::GetInstance()->SetErrorLine(leneNum);
				return;
			}
			tokenGroups.push_back({{token}, TokenGroupType::Function});
			continue;
		}
		// 制御構文
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

		// コマンド
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
		if(token.front() == '#' || token.front() == '$') {
			if (commandTokens.type == TokenGroupType::None) {
				// コマンドがまだ決まっていない場合、コマンド名として扱う
				commandTokens.tokens.push_back(token);
				commandTokens.tokens.push_back(":");
				commandTokens.type = TokenGroupType::Command;
			} else {
				// ここに来るのはおかしい
				ErrorMessage::GetInstance()->SetMessage(U"どんな書き方したの？");
				ErrorMessage::GetInstance()->SetErrorLine(leneNum);
				return;
			}
		}
		preToken = token;
	}
	// 最後のトークンを追加
	if (!commandTokens.tokens.empty() && commandTokens.type != TokenGroupType::None) {
		tokenGroups.push_back(commandTokens);
	}
}

bool CardData::AdaptationCommand(int i) {
	// ネストの中でないとコマンドは使えない
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストに入ってないよ");
		return false;
	}
	std::string pre;
	std::string key;
	std::vector<std::string> commandTokens;
	bool isCommand = false;
	for (const auto& token : tokenGroups[i].tokens) {
		// コマンドの引数
		if (isCommand) {
			commandTokens.push_back(token);
		}
		// コマンドのキー
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
	// ネストの中でないとifは使えない
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストに入ってないよ");
		return false;
	}
	cardCommands[nestStack.top()].push_back(nullptr); // 仮のNULLコマンドを追加
	int commandIndex = static_cast<int>(cardCommands[nestStack.top()].size()) - 1;
	int nextNestID = newNestID;
	int nestIndex = nestStack.top();
	std::vector<std::string> conditionTokens;
	if (!ParseBool(i, conditionTokens, U"if")) {
		return false;
	}

	int lineNumber = lineNum;
	commandQueue.push([this, commandIndex, nestIndex, nextNestID, conditionTokens, lineNumber]() {
		if (cardCommands[nextNestID].size() <= 0) {
			ErrorMessage::GetInstance()->SetMessage(U"ifの中身がないよ");
			return false;
		}
		std::unique_ptr<CardCommand> ifCommand = CardCommandFactory::CreateIfCommand(this, nextNestID, conditionTokens);
		if (ifCommand == nullptr) {
			ErrorMessage::GetInstance()->SetErrorLine(lineNumber);
			return false;
		}
		cardCommands[nestIndex][commandIndex] = std::move(ifCommand);
		return true;
	});
	return true;
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
	// 関数定義はネストの外でしかできない
	if (tokenGroups[i + 1].type != TokenGroupType::NestStart) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストが見つからないよ");
		return false;
	}
	// ネストの中で関数定義はできない
	if(!nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネスト内で関数定義はできないよ");
		return false;
	}
	std::string functionName = tokenGroups[i].tokens[0].substr(1); // '@'を除去

	if (functionMap.contains(functionName)) {
		ErrorMessage::GetInstance()->SetMessage(U"同じ名前の関数が既に存在するよ");
		return false;
	}
	// 新しいネストIDをスタックに追加して、そのIDを関数名と紐づける
	functionMap[functionName] = newNestID;

	return true;
}

bool CardData::AdaptationNestStart(int i) {
	// ネストの開始は関数定義の直後か、ネストの中でしか使えない
	nestStack.push(newNestID);
	newNestID++;
	return true;
}

bool CardData::AdaptationNestEnd(int i) {
	// ネストの終了はネストの中でしか使えない
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストの終わりが多いよ");
		return false;
	}
	int index = nestStack.top();
	nestStack.pop();
	//if(!nestStack.empty()){
	//	std::unique_ptr<CardCommand> command = CardCommandFactory::CreateNestMoveCommand(nestStack.top());
	//	cardCommands[index].push_back(std::move(command));
	//}
	return true;
}

bool CardData::AdaptationReturn(int i) {
	// ネストの中でないとreturnは使えない
	if (nestStack.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"ネストに入ってないよ");
		return false;
	}
	std::unique_ptr<CardCommand> command = CardCommandFactory::CreateReturnCommand();
	cardCommands[nestStack.top()].push_back(std::move(command));
	return true;
}

bool CardData::ParseBool(int i, std::vector<std::string>& conditionTokens, std::u32string str) {
	bool is = false;
	bool is2 = false;
	int count = 0;
	for (const auto& token : tokenGroups[i].tokens) {
		// ifの条件式
		if (token == "(") {
			if (count == 0) {
				if (!is) {
					ErrorMessage::GetInstance()->SetMessage(str + U"の後にかっこが無いよ");
					return false;
				}
				is2 = true;
				count++;
				continue;
			}
			count++;
		}

		if (token == ")") {
			count--;
			if (count <= 0) {
				if (count == 0) {
					continue;
				}
				ErrorMessage::GetInstance()->SetMessage(U"かっこの終わりが多いよ");
				return false;
			}
		}

		if (token != "if") {
			conditionTokens.push_back(token);
			is = false;
			continue;
		}
		is = true;
	}
	if (!is2) {
		ErrorMessage::GetInstance()->SetMessage(U"かっこがないよ");
		return false;
	}

	if (count > 0) {
		ErrorMessage::GetInstance()->SetMessage(U"かっこの終わりが足らないよ");
		return false;
	}
	return true;
}
