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

enum class CardCharacteristics {
	Eraser,          // 消滅
	Collapse,        // 崩壊
	Temporary,      // 臨時
	Protection,     // 保護
	TemporaryProtection, // 一時保護
	Error,          // エラー用の要素
};
enum class CardRarity {
	Common,
	Uncommon,
	Epic,
	Legendary,
	Error,
};

enum class BuildingDurabilityDecreasenTiming {
	None,
	Effect,
	EndTurn,
	Error,
};

enum class BuildingActivationTiming {
	None,
	StartTurn,
	EndTurn,
	Manual,
	Separated,
	Break,
	Error,
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
	void SetWaku(bool isWaku) {
		isWaku_ = isWaku;
	}
	bool IsWaku() const {
		return isWaku_;
	}
	void InitializeDurability() {
		durability_ = durabilityInitial_;
	}
private:
	void UpdateDesription();
	
private:
	std::unique_ptr<Text> name_ = nullptr;
	std::u32string descriptionstr_;
	std::unique_ptr<Text> description_ = nullptr;
	CardElement element_ = CardElement::None;
	CardType type_ = CardType::Error; // カードの種類（儀式、建物など）
	int cost_ = 0; // コスト
	CardElement elementCost_ = CardElement::Error; // コストの要素
	CardRarity rarity_ = CardRarity::Error; // レアリティ

	CardData* cardData_ = nullptr;

	std::string fileName = "white.png";
	// カード特性OnOff
	//　消滅
	bool isEraser_ = false;
	// 崩壊
	bool isCollapse_ = false;
	// 臨時
	bool isTemporary_ = false;
	// 保護
	bool isProtection_ = false;
	// 一時保護
	bool isTemporaryProtection_ = false;

	BuildingDurabilityDecreasenTiming buildingDurabilityDecreasenTiming_ = BuildingDurabilityDecreasenTiming::None;

	BuildingActivationTiming buildingActivationTiming_ = BuildingActivationTiming::None;

	int durability_ = 0; // 耐久値
	int durabilityInitial_ = 0; // 初期耐久値
private:
	CardZone zone_ = CardZone::None;

	CardManager* cardManager_;

	bool isDraw_ = false;
	std::unique_ptr<Sprite> sprite_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Sprite> spritewaku_ = nullptr;
	std::unique_ptr<Sprite> costSprite_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Sprite> costSprite2_ = nullptr; // スプライト（カードの画像）
	std::unique_ptr<Text> costText_ = nullptr; // コストテキスト
	Vector2 halfSize = {60.0f, 80.0f};
	Vector2 pos = {-100.0f, -100.0f};
	const Vector2 textZure = {0.0f, -80.0f};
	bool isMove = false;
	bool isCommandMove_ = false;

	bool isWaku_ = false;

	// コマンド実行形式の判別用
	bool isVoid_ = false;

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

	bool IsVoid() const {
		return isVoid_;
	}

public:
	// 
	void SetName(const std::u32string& name) {
		name_->SetText(name);
	}
	void SetDescription(const std::u32string& description) {
		descriptionstr_ = description;
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
	void SetIsCardCharacteristics(CardCharacteristics characteristics, bool isSet);

	bool GetIsCardCharacteristics(CardCharacteristics characteristics) const;

	void SetRarity(CardRarity rarity) {
		rarity_ = rarity;
	}
	CardRarity GetRarity() const {
		return rarity_;
	}

	void SetBuildingDurabilityDecreasenTiming(BuildingDurabilityDecreasenTiming timing) {
		buildingDurabilityDecreasenTiming_ = timing;
	}
	BuildingDurabilityDecreasenTiming GetBuildingDurabilityDecreasenTiming() const {
		return buildingDurabilityDecreasenTiming_;
	}

	void SetBuildingActivationTiming(BuildingActivationTiming timing) {
		buildingActivationTiming_ = timing;
	}
	BuildingActivationTiming GetBuildingActivationTiming() const {
		return buildingActivationTiming_;
	}

	void SetDurability(int durability) {
		durabilityInitial_ = durability;
	}
	int GetDurability() const {
		return durability_;
	}

	bool IsCostSufficient(int& light, int& darkness);

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

