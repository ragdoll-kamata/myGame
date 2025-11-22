#pragma once
#include "Card.h"
#include <string>
#include <vector>

enum class ExecuteResult {
	Normal,
	Standby,
	Return,
	Break,
	Continue,
	Error,
};
class CardCommand {
public:
	enum class ParseBoolType {
		Element,
		Type,
		Int,
		Name,
		Bool,
		Operators,
	};

	struct ParseBoolData {
		ParseBoolType type;
		std::string value;
		bool reverse = false;

	};
	struct ParseBoolResult;
	struct ParseBoolGroup {
		std::vector<ParseBoolData> dates;
		bool chain; // true: &&, false: ||
		std::unique_ptr<ParseBoolResult> next = nullptr;
	};
	struct ParseBoolResult {
		std::vector<ParseBoolGroup> groups;
	};
	enum class IntExprNodeType {
		Num,
		Add,
		Subtract,
		Multiply,
		Division,
		None,
	};

	struct ExprNode {
		IntExprNodeType type = IntExprNodeType::None;
		std::string str;
		std::unique_ptr<ExprNode> left, right;
	};
	

	


	/// <summary>
	/// カードコマンド実行
	/// </summary>
	/// <param name="card">カード</param>
	/// <returns>リザルト</returns>
	virtual ExecuteResult Execute(Card* card) = 0;

	static void SetCardManager(CardManager* cardManager) {
		cardManager_ = cardManager;
	}
protected:
	int ParseInt(std::string num, Card* card);
	bool ParseCard(std::string& cardNum, std::vector<Card*>& cards, Card* card);
	CardElement ParseCardElement(std::string element, Card* card);
	CardType ParseCardType(std::string type, Card* card);
	std::u32string Utf8ToU32(const std::string& str);

	std::unique_ptr<ParseBoolResult> ParseBool(std::vector<std::string>& boolTokens);

	bool ExecuteBool(std::unique_ptr<ParseBoolResult>& parseBoolResult, Card* card);

	bool Parse(std::string str, std::vector<std::string>& token);

	std::unique_ptr<ExprNode> CreateIntExprNode(std::vector<std::string>& tokens);

	int CalculationIntExprNode(std::unique_ptr<ExprNode>& root, Card* card);

	std::unique_ptr<ExprNode> CreateCardExprNode(std::vector<std::string>& tokens);

	bool CalculationCardExprNode(std::unique_ptr<ExprNode>& root, std::vector<Card*>& cards, Card* card);
private:
	void ExprNodeSet(std::unique_ptr<ExprNode>& root, std::unique_ptr<ExprNode>& node);
	
	

protected:
	static CardManager* cardManager_;
};

