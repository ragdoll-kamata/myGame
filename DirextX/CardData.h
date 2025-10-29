#pragma once
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include "CardCommand.h"
enum class TokenGroupType {
	Command,
	If,
	ElseIf,
	Else,
	While,
	For,
	Function,
	NestStart,
	NestEnd,
	Return,
	None,
};
struct TokenGroup {
	std::vector<std::string> tokens;
	TokenGroupType type = TokenGroupType::None;
	int lineNumber = 0;
};
class CardData {
public:
	void LoadCardFile(std::string filename);

	bool CardFunctionLoad(Card* card, std::string functionName);

	std::vector<CardCommand*> GetCardCommands(int functionID);

private:
	bool FunctionLoad(Card* card, int functionID);

	std::vector<std::string> ParseLine(std::string& text);

	void CreateTokenGroup(std::vector<std::string>& tokens, int leneNum);





	bool AdaptationCommand(int i);
	bool AdaptationIf(int i);
	bool AdaptationElseIf(int i);
	bool AdaptationElse(int i);
	bool AdaptationWhile(int i);
	bool AdaptationFor(int i);
	bool AdaptationFunction(int i);
	bool AdaptationNestStart(int i);
	bool AdaptationNestEnd(int i);
	bool AdaptationReturn(int i);

	bool ParseBool(int i, std::vector<std::string>& conditionTokens, std::u32string str);

private:
	std::unordered_map<TokenGroupType, std::function<bool(int)>> tokenGroupFunctions{
		{TokenGroupType::Command,   [&](int i) {return AdaptationCommand(i);   }},
		{TokenGroupType::If,        [&](int i) {return AdaptationIf(i);        }},
		{TokenGroupType::ElseIf,    [&](int i) {return AdaptationElseIf(i);    }},
		{TokenGroupType::Else,      [&](int i) {return AdaptationElse(i);      }},
		{TokenGroupType::While,     [&](int i) {return AdaptationWhile(i);     }},
		{TokenGroupType::For,       [&](int i) {return AdaptationFor(i);       }},
		{TokenGroupType::Function,  [&](int i) {return AdaptationFunction(i);  }},
		{TokenGroupType::NestStart, [&](int i) {return AdaptationNestStart(i); }},
		{TokenGroupType::NestEnd,   [&](int i) {return AdaptationNestEnd(i);   }},
		{TokenGroupType::Return,    [&](int i) {return AdaptationReturn(i);    }},
	};
	std::vector<TokenGroup> tokenGroups;
	int newNestID = 1;

	std::stack<int> nestStack;

	std::queue<std::function<bool()>> commandQueue;
	


	//void CommandAdaptation(std::string key, std::vector<std::string> commandTokens);
	std::unordered_map<std::string, int> functionMap;

	std::unordered_map<int, std::vector<std::unique_ptr<CardCommand>>> cardCommands;

	int lineNum = 0;

	//int newFunctionId = 0;
	//int nowFunctionId = -1;
};

