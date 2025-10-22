#pragma once
#include "Card.h"
#include <string>
#include <vector>

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
	};
	struct ParseBoolGroup {
		std::vector<ParseBoolData> datas;
		bool chein; // true: &&, false: ||
	};
	struct ParseBoolResult {
		std::vector<ParseBoolGroup> groups;
	};

	virtual int Execute(Card* card) = 0;

	int ParseInt(std::string num, Card* card);
	CardElement ParseCardElement(std::string element, Card* card);
	CardType ParseCardType(std::string type, Card* card);
	std::u32string Utf8ToU32(const std::string& str);

	ParseBoolResult ParseBool(std::vector<std::string>& boolTokens);

	bool ExecuteBool(ParseBoolResult& parseBoolResult, Card* card);

	static void SetCardManager(CardManager* cardManager) {
		cardManager_ = cardManager;
	}
protected:
	static CardManager* cardManager_;
};

