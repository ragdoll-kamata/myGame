#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include <Sprite.h>
#include <Text.h>

class CardManager;
class CardData;

enum class CardZone {
	Hand,
	Deck,
	Cemetery,
	Execution,
	Field,
	Open,
	Void,
	None,
};
enum class CardElement {
	Light,
	Darkness,
	None,
	Error, // エラー用の要素
};

enum class CardType {
	Ritual,
	Building,
	Error, // エラー用の要素
};


class Card {
public:
	bool InitializeCard(CardData* loadCard);

	void Update();

	void Draw();
	void TextDraw();

	void EffectTextDraw();

	bool IsOnCollision(Vector2 pos);

	bool Effect();
public:

	void SetZone(CardZone zone) {
		zone_ = zone;
	}
	CardZone GetZone() const {
		return zone_;
	}

	void SetIsDraw(bool isDraw) {
		isDraw_ = isDraw;
	}
	bool IsDraw() const {
		return isDraw_;
	}
	void SetIsMove(bool isMove) {
		this->isMove = isMove;
	}
	void SetIsCommandMove(bool isCommandMove) {
		isCommandMove_ = isCommandMove;
	}
	bool IsCommandMove() const {
		return isCommandMove_;
	}
private:
	std::unique_ptr<Text> name_ = nullptr;
	std::unique_ptr<Text> description_ = nullptr;
	CardElement element_ = CardElement::None;
	CardType type_ = CardType::Error; // カードの種類（儀式、建物など）
	int cost_ = 0; // コスト
	CardElement elementCost_ = CardElement::None; // コストの要素
	CardData* cardData_ = nullptr;

	std::string fileName = "white.png";
	//std::vector<CardCommand*> effectCommands; // 効果コマンドのリスト
private:
	CardZone zone_ = CardZone::None;

	CardManager* cardManager_;

	bool isDraw_ = false;
	std::unique_ptr<Sprite> sprite_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Sprite> costSprite_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Sprite> costSprite2_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Text> costText_ = nullptr; // コストテキスト
	Vector2 halfSize = {60.0f, 80.0f};
	Vector2 pos = {-100.0f, -100.0f};
	const Vector2 textZure = {0.0f, -80.0f};
	bool isMove = false;
	bool isCommandMove_ = false;

	int functionLine = 0;
public:

	void SetPos(Vector2& pos);
	Vector2 GetPos() {
		return sprite_->GetPosition();
	}
	void SetNewPos(Vector2& newPos) {
		pos = newPos;
		isMove = true;
	}
	bool IsMove() const {
		return isMove;
	}

public:
	// 
	void SetName(const std::u32string& name) {
		if (!name_) {
			name_ = std::make_unique<Text>();
		}
		name_->SetText(name);
	}
	void SetDescription(const std::u32string& description) {
		if (!description_) {
			description_ = std::make_unique<Text>();
			description_->Initialize(description, {}, 120.0f);
		}
		description_->SetText(description);
	}
	CardElement GetElement() const {
		return element_;
	}
	void SetElement(CardElement element) {
		element_ = element;
	}
	CardType GetType() const {
		return type_;
	}
	void SetType(CardType type) {
		type_ = type;
	}
	int GetCost() const {
		return cost_;
	}
	void SetCost(int cost) {
		cost_ = cost;
	}
	void SetElementCost(CardElement element) {
		elementCost_ = element;
	}
	CardElement GetElementCost() const {
		return elementCost_;
	}
	void SetFileName(std::string name) {
		fileName = name;
	}
	std::string GetFileName() {
		return fileName;
	}

private:
// 参照変数
	std::unordered_map<std::string, std::vector<Card*>> cards;
	std::unordered_map<std::string, int> ints;
	std::string returnValue;
public:
	// 変数
	void SetInt(const std::string& key, int value) {
		ints[key] = value;
	}
	int GetInt(const std::string& key) const {
		auto it = ints.find(key);
		if (it != ints.end()) {
			return it->second;
		}
		return 0; // or throw an exception, or handle as needed
	}
	void AddInt(const std::string& key, int value) {
		ints[key] += value;
	}
	void SubtractInt(const std::string& key, int value) {
		ints[key] -= value;
		if (ints[key] < 0) {
			ints[key] = 0; // prevent negative values
		}
	}
	void MultiplyInt(const std::string& key, int value) {
		ints[key] *= value;
	}
	void DivideInt(const std::string& key, int value) {
		if (value != 0) {
			ints[key] /= value;
		} else {
			// Handle division by zero as needed, e.g., throw an exception or log an error
		}
	}

	void SetCards(const std::string& key, const std::vector<Card*>& value) {
		cards[key] = value;
	}
	std::vector<Card*> GetCards(const std::string& key) const {
		auto it = cards.find(key);
		if (it != cards.end()) {
			return it->second;
		}
		return {}; // return an empty vector if the key is not found
	}
	void AddCard(const std::string& key, Card* card) {
		cards[key].push_back(card);
	}
	void RemoveCard(const std::string& key, Card* card) {
		if(!cards.contains(key)) {
			return;
		}
		auto& cardList = cards[key];
		cardList.erase(std::remove(cardList.begin(), cardList.end(), card), cardList.end());
	}

	std::string GetReturnValue() const {
		return returnValue;
	}
	void SetReturnValue(const std::string& value) {
		returnValue = value;
	}
	void ResetReturnValue() {
		returnValue.clear();
	}

	void RessetVariable() {
		cards.clear();
		ints.clear();
		returnValue.clear();
	}

};

