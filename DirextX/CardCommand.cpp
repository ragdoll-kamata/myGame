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
		if (num.front() == '$') {
			if (card == nullptr) {
				return -1;
			}
			size_t pos = num.find('.');
			std::string str = num.substr(pos + 1);
			if (str == "枚数") {
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

	if (type.front() == '$') {
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

std::unique_ptr<CardCommand::ParseBoolResult> CardCommand::ParseBool(std::vector<std::string>& boolTokens) {
	// 真偽解析
	std::unique_ptr<ParseBoolResult> parseBoolResult = std::make_unique<ParseBoolResult>();
	int index = 0;
	parseBoolResult->groups.push_back(ParseBoolGroup());
	bool isGroup = false;
	// 括弧ように変数コピー
	std::vector<std::string> subTokens;
	subTokens = boolTokens;

	int skipCount = 0;
	for (const auto& token : boolTokens) {
		// 括弧の処理のための先頭廃棄
		subTokens.erase(subTokens.begin());

		
		if (isGroup) {
			if (token == ")") {
				skipCount--;
			}
			if (skipCount > 0) {
				continue;
			}
			skipCount = 0;
			isGroup = false;
			continue;
		}

		ParseBoolData data;
		// 括弧がある場合、閉じ括弧を探してその間のトークンで再起してparseBoolResult->groups[index]のnext変数に入れる
		if (token == "(") {
			int depth = 1;
			std::vector<std::string> subTokens2;
			isGroup = true;
			skipCount = 1;
			for (const auto& token : subTokens) {
				subTokens2.push_back(token);
				if (token == "(") {
					depth++;
					skipCount++;
					continue;
				}

				if (token == ")") {
					depth--;
					if (depth == 0) {
						subTokens2.erase(subTokens2.end()-1);
						parseBoolResult->groups[index].next = ParseBool(subTokens2);
						if (parseBoolResult->groups[index].next == nullptr) {
							return nullptr;
						}
						parseBoolResult->groups[index].dates.push_back(data);
						break;
					}

				}
			}
			continue;
		}

		data.value = token;

		if (token.front() == '!' && token != "!=") {
			data.reverse = true;
			data.value = token.substr(1);
		}

		//// 結合子の処理
		if (token == "&&") {
			parseBoolResult->groups[index].chain = true;
			parseBoolResult->groups.push_back(ParseBoolGroup());
			index++;
			continue;
		}
		if (token == "||") {
			parseBoolResult->groups[index].chain = false;
			parseBoolResult->groups.push_back(ParseBoolGroup());
			index++;
			continue;
		}
		if (token == "==" || token == "<=" || token == "<" || token == ">=" || token == ">" || token == "!=") {
			data.type = ParseBoolType::Operators;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}


		//// 変換できる先を探す

		// 変数の場合
		if (token.front() == '#') {
			data.type = ParseBoolType::Int;
			data.value = token;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}
		if (token.front() == '$') {
			size_t pos = token.find('.');
			std::string str = token.substr(pos + 1);
			if (str == "枚数") {
				data.type = ParseBoolType::Int;
				parseBoolResult->groups[index].dates.push_back(data);
				continue;
			}
			if (str == "属性") {
				data.type = ParseBoolType::Element;
				parseBoolResult->groups[index].dates.push_back(data);
				continue;
			}
			if (str == "タイプ") {
				data.type = ParseBoolType::Type;
				parseBoolResult->groups[index].dates.push_back(data);
				continue;
			}

		}
		// 固定値の場合

		if (token == "true" || token == "True") {
			data.type = ParseBoolType::Bool;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}
		if (token == "false" || token == "False") {
			data.type = ParseBoolType::Bool;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}

		CardElement element = ParseCardElement(token, nullptr);

		if (element != CardElement::Error) {
			data.type = ParseBoolType::Element;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}
		CardType type = ParseCardType(token, nullptr);
		if (type != CardType::Error) {
			data.type = ParseBoolType::Type;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}
		int i = ParseInt(token, nullptr);
		if (i != -1) {
			data.type = ParseBoolType::Int;
			parseBoolResult->groups[index].dates.push_back(data);
			continue;
		}
		return nullptr;
	}
	return std::move(parseBoolResult);
}

bool CardCommand::ExecuteBool(std::unique_ptr<ParseBoolResult>& parseBoolResult, Card* card) {
	int index = 0;
	for (auto& group : parseBoolResult->groups) {
		bool result = false;
		if (group.next) {
			result = ExecuteBool(group.next, card);
		} else {
			if (group.dates.size() != 3 && group.dates[0].type != ParseBoolType::Bool) {
				return false;
			}
			if (group.dates[0].type != ParseBoolType::Bool && group.dates[1].type != ParseBoolType::Operators) {
				return false;
			}
			
			// 比較処理
			if (group.dates[0].type != ParseBoolType::Bool) {
				if (group.dates[0].type == group.dates[2].type) {
					if (group.dates[0].type == ParseBoolType::Int) {
						// 整数の比較
						int left = ParseInt(group.dates[0].value, card);
						int right = ParseInt(group.dates[2].value, card);
						if (group.dates[1].value == "==") {
							result = (left == right);
						} else if (group.dates[1].value == "!=") {
							result = (left != right);
						} else if (group.dates[1].value == "<") {
							result = (left < right);
						} else if (group.dates[1].value == "<=") {
							result = (left <= right);
						} else if (group.dates[1].value == ">") {
							result = (left > right);
						} else if (group.dates[1].value == ">=") {
							result = (left >= right);
						}
					} else if (group.dates[0].type == ParseBoolType::Element) {
						// 属性の比較
						CardElement left = ParseCardElement(group.dates[0].value, card);
						CardElement right = ParseCardElement(group.dates[2].value, card);
						if (group.dates[1].value == "==") {
							result = (left == right);
						} else if (group.dates[1].value == "!=") {
							result = (left != right);
						}
					} else if (group.dates[0].type == ParseBoolType::Type) {
						// タイプの比較
						CardType left = ParseCardType(group.dates[0].value, card);
						CardType right = ParseCardType(group.dates[2].value, card);
						if (group.dates[1].value == "==") {
							result = (left == right);
						} else if (group.dates[1].value == "!=") {
							result = (left != right);
						}
					}
				}
			} else {
				// 真偽値の処理
				if (group.dates.size() == 1) {
					// 真偽値の処理
					bool left = (group.dates[0].value == "true" || group.dates[0].value == "True") ? true : false;
					if (group.dates[0].reverse) {
						left = !left;
					}
					result = left;
				} else {
					// 真偽値の比較
					bool left = (group.dates[0].value == "true" || group.dates[0].value == "True") ? true : false;
					bool right = (group.dates[2].value == "true" || group.dates[2].value == "True") ? true : false;
					if (group.dates[0].reverse) {
						left = !left;
					}
					if (group.dates[2].reverse) {
						right = !right;
					}
					if (group.dates[1].value == "==") {
						result = (left == right);
					} else if (group.dates[1].value == "!=") {
						result = (left != right);
					}
				}

			}
		}

		// 結合処理
		if (index < parseBoolResult->groups.size() - 1) {
			if (parseBoolResult->groups[index].chain) {
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