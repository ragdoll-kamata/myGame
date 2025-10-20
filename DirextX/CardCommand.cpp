#include "CardCommand.h"
#include <stdexcept>
#include "utf8.h"

CardManager* CardCommand::cardManager_ = nullptr;

int CardCommand::ParseInt(std::string num, Card* card) {
	if (num.front() == '#') {
		// 変数の場合
		return card->GetInt(num);
	} else if (num != "") {
		// 固定値の場合
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
		// 数字に変換できる場合
		try {
			int value = std::stoi(num);
			return value;
		} catch (const std::invalid_argument& e) {
			e;
		} catch (const std::out_of_range& e) {
			e;
		}
	}
	// エラーの場合
	return -1;
}

CardElement CardCommand::ParseCardElement(std::string element, Card* card) {
	// 直接属性名が指定されている場合
	if (element == "光属性") {
		return CardElement::Light;
	} else if (element == "闇属性") {
		return CardElement::Darkness;
	} else if (element == "無属性") {
		return CardElement::None;
	}
	if (element.front() == '$') {
		//　カード変数の属性を取得する処理
		std::string key;
		std::string cardElement;
		for (const auto& c : element) {
			if (c == '.') {
				cardElement += c;
			} else {
				key += c;
			}
		}
		if (cardElement == "属性") {
			std::vector<Card*> cards = card->GetCards(key);
			if (cards.size() > 0) {
				return cards[0]->GetElement();
			}
		}
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

bool CardCommand::ParseBool(std::vector<std::string>& boolTokens, Card* card) {
	// 真偽解析
	std::vector<std::vector<ParseBoolData>> boolTokenGroups;
	std::vector<bool> cheins;
	int index = 0;
	for (const auto& token : boolTokens) {
		// 結合子の処理
		if (token == "&&") {
			index++;
			cheins.push_back(true);

		} else if (token == "||") {
			index++;
			cheins.push_back(false);

		} /*else if (token == "==") {

		} else if (token == "<=") {

		} else if (token == "<") {

		} else if (token == ">=") {

		} else if (token == ">") {

		} else if (token == "!=") {

		}*/


		// 変換できる先を探す
		
		if (token.front() == '#') {
			

		}else if (token.front() == '$') {
			size_t pos = token.find('.');
			std::string str = token.substr(pos + 1);
			if (str == "枚数") {
			} else if (str == "")

		}
		CardElement element = ParseCardElement(token, card);
		
		
		if(element != CardElement::Error) {
		}
		CardType type = ParseCardType(token);
		if (type != CardType::Error) {
		}
		int i = ParseInt(token, card);
		if (i != -1) {
		} 

	}



	return false;
}
