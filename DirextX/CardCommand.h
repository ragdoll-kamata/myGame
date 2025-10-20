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
	virtual int Execute(Card* card) = 0;

	int ParseInt(std::string num, Card* card);
	CardElement ParseCardElement(std::string element, Card* card = nullptr);
	CardType ParseCardType(std::string type);
	std::u32string Utf8ToU32(const std::string& str);

	bool ParseBool(std::vector<std::string>& boolTokens, Card* card);

	static void SetCardManager(CardManager* cardManager) {
		cardManager_ = cardManager;
	}
protected:
	static CardManager* cardManager_;
};

