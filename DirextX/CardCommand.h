#pragma once
#include "Card.h"
#include <string>

class CardCommand {
public:
	virtual int Execute(Card* card) = 0;

	int ParseInt(std::string num, Card* card);
	CardElement ParseCardElement(std::string element);
	CardType ParseCardType(std::string type);
	std::u32string Utf8ToU32(const std::string& str);
};

