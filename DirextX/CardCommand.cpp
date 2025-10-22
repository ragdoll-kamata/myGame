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
		// カード変数の場合
		if(num.front() == '$') {
			if(card == nullptr) {
				return -1;
			}
			size_t pos = num.find('.');
			std::string str = num.substr(pos + 1);
			if(str == "枚数") {
				std::string key = num.substr(0, pos);
				std::vector<Card*> cards = card->GetCards(key);
				return static_cast<int>(cards.size());
			}
			
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
	}
	if (element == "闇属性") {
		return CardElement::Darkness;
	}
	if (element == "無属性") {
		return CardElement::None;
	}
	if (element.front() == '$') {
		if (card == nullptr) {
			return CardElement::Error;
		}
		//　カード変数の属性を取得する処理
		size_t pos = element.find('.');
		std::string str = element.substr(pos + 1);
		if (str == "属性") {
			std::string key = element.substr(0, pos);
			std::vector<Card*> cards = card->GetCards(key);
			if (cards.size() > 0) {
				return cards[0]->GetElement();
			}
		}
	}

	return CardElement::Error;
}

CardType CardCommand::ParseCardType(std::string type, Card* card) {
	if (type == "儀式") {
		return CardType::Ritual;
	} else if (type == "建物") {
		return CardType::Building;
	}

	if(type.front() == '$') {
		if (card == nullptr) {
			return CardType::Error;
		}
		size_t pos = type.find('.');
		std::string str = type.substr(pos + 1);
		if (str == "タイプ") {
			std::string key = type.substr(0, pos);
			std::vector<Card*> cards = card->GetCards(key);
			return cards.front()->GetType();
		}
	}
	return CardType::Error;
}

std::u32string CardCommand::Utf8ToU32(const std::string& str) {
	std::u32string u32_str;
	utf8::utf8to32(str.begin(), str.end(), std::back_inserter(u32_str));
	return u32_str;
}

CardCommand::ParseBoolResult CardCommand::ParseBool(std::vector<std::string>& boolTokens) {
	// 真偽解析
	ParseBoolResult parseBoolResult;
	int index = 0;
	parseBoolResult.groups.push_back(ParseBoolGroup());
	for (const auto& token : boolTokens) {
		ParseBoolData data;
		//// 結合子の処理
		if (token == "&&") {
			parseBoolResult.groups[index].chein = true;
			parseBoolResult.groups.push_back(ParseBoolGroup());
			index++;
			continue;
		}
		if (token == "||") {
			parseBoolResult.groups[index].chein = false;
			parseBoolResult.groups.push_back(ParseBoolGroup());
			index++;
			continue;
		}
		if (token == "==" || token == "<=" || token == "<" || token == ">=" || token == ">" || token == "!=") {
			data.type = ParseBoolType::Operators;
			data.value = token;
			parseBoolResult.groups[index].datas.push_back(data);
			continue;
		}


		//// 変換できる先を探す
		
		// 変数の場合
		if (token.front() == '#') {
			data.type = ParseBoolType::Int;
			data.value = token;
			parseBoolResult.groups[index].datas.push_back(data);
			continue;
		}
		if (token.front() == '$') {
			size_t pos = token.find('.');
			std::string str = token.substr(pos + 1);
			if (str == "枚数") {
				data.type = ParseBoolType::Int;
				data.value = token;
				parseBoolResult.groups[index].datas.push_back(data);
				continue;
			}
			if (str == "属性") {
				data.type = ParseBoolType::Element;
				data.value = token;
				parseBoolResult.groups[index].datas.push_back(data);
				continue;
			}
			if (str == "タイプ") {
				data.type = ParseBoolType::Type;
				data.value = token;
				parseBoolResult.groups[index].datas.push_back(data);
				continue;
			}

		}
		// 固定値の場合
		CardElement element = ParseCardElement(token, nullptr);
		
		if(element != CardElement::Error) {
			data.type = ParseBoolType::Element;
			data.value = token;
			parseBoolResult.groups[index].datas.push_back(data);
			continue;
		}
		CardType type = ParseCardType(token, nullptr);
		if (type != CardType::Error) {
			data.type = ParseBoolType::Type;
			data.value = token;
			parseBoolResult.groups[index].datas.push_back(data);
			continue;
		}
		int i = ParseInt(token, nullptr);
		if (i != -1) {
			data.type = ParseBoolType::Int;
			data.value = token;
			parseBoolResult.groups[index].datas.push_back(data);
			continue;
		} 
		return ParseBoolResult();
	}
	return parseBoolResult;
}

bool CardCommand::ExecuteBool(ParseBoolResult& parseBoolResult, Card* card) {
	int index = 0;
	for (const auto& group : parseBoolResult.groups) {
		if (group.datas.size() != 3) {
			return false;
		}
		if (group.datas[1].type != ParseBoolType::Operators) {
			return false;
		}
		bool result = false;
		// 比較処理
		if (group.datas[0].type == group.datas[2].type) {
			if (group.datas[0].type == ParseBoolType::Int) {
				// 整数の比較
				int left = ParseInt(group.datas[0].value, card);
				int right = ParseInt(group.datas[2].value, card);
				if (group.datas[1].value == "==") {
					result = (left == right);
				} else if (group.datas[1].value == "!=") {
					result = (left != right);
				} else if (group.datas[1].value == "<") {
					result = (left < right);
				} else if (group.datas[1].value == "<=") {
					result = (left <= right);
				} else if (group.datas[1].value == ">") {
					result = (left > right);
				} else if (group.datas[1].value == ">=") {
					result = (left >= right);
				}
			} else if (group.datas[0].type == ParseBoolType::Element) {
				// 属性の比較
				CardElement left = ParseCardElement(group.datas[0].value, card);
				CardElement right = ParseCardElement(group.datas[2].value, card);
				if (group.datas[1].value == "==") {
					result = (left == right);
				} else if (group.datas[1].value == "!=") {
					result = (left != right);
				}
			} else if (group.datas[0].type == ParseBoolType::Type) {
				// タイプの比較
				CardType left = ParseCardType(group.datas[0].value, card);
				CardType right = ParseCardType(group.datas[2].value, card);
				if (group.datas[1].value == "==") {
					result = (left == right);
				} else if (group.datas[1].value == "!=") {
					result = (left != right);
				}
			}


		}
		// 結合処理
		if (index < parseBoolResult.groups.size() - 1) {
			if (parseBoolResult.groups[index].chein) {
				// AND
				if (!result) {
					return false;
				}
			} else {
				// OR
				if (result) {
					return true;
				}
			}
			index++;
		} else {

			return result;
		}
	}
	return false;
}