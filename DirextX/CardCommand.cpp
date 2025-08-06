#include "CardCommand.h"
#include <stdexcept>
#include "utf8.h"

int CardCommand::ParseInt(std::string num, Card* card) {
	if (num.front() == '#') {
		return card->GetInt(num);
	} else if(num != "") {
		if (num == "光エネルギー") {

		}
		if (num == "闇エネルギー") {

		}
		if (num == "山札枚数") {

		}
		if (num == "墓地枚数") {

		}
		if (num == "手札枚数") {

		}
		if (num == "スコア") {

		}
		try {
			int value = std::stoi(num);
			return value;
		} catch (const std::invalid_argument& e) {
			e;
		} catch (const std::out_of_range& e) {
			e;
		}
	}
	return -1;
}

CardElement CardCommand::ParseCardElement(std::string element) {
	if (element == "光属性") {
		return CardElement::Light;
	} else if (element == "闇属性") {
		return CardElement::Darkness;
	} else if (element == "無属性") {
		return CardElement::None;
	} 
	return CardElement::Error;
}

CardType CardCommand::ParseCardType(std::string type) {
	if (type == "儀式") {
		return CardType::Ritual;
	} else if (type == "建物") {
		return CardType::Building;
	}
	return CardType::Error;
}

std::u32string CardCommand::Utf8ToU32(const std::string& str) {
	std::u32string u32_str;
	utf8::utf8to32(str.begin(), str.end(), std::back_inserter(u32_str));
	return u32_str;
}