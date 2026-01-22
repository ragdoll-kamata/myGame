#pragma once
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <memory>
#include <unordered_map>
#include <random>
#include <functional>

#include "Card.h"
#include "CardData.h"
#include "Button.h"
#include "TrunState.h"
#include "CardMove.h"

struct CardFile {
	std::string modName;
	std::string filePath;
	std::string fileName;
};
struct CardMoveData {
	Card* card;
	Vector2 pos;
};
class UIManager;
class ResourceManager;

class CardManager {
public:
	void Initialize(UIManager* uiManager, ResourceManager* resourceManager);

	bool StartCardSet();

	void Update(TurnState& trunState);

	void Draw();
	void SelectDraw();

	void TextDraw();

	void AddCardMove(std::vector<std::unique_ptr<CardMove>> moveCard);
	std::unordered_map<std::string, std::unique_ptr<CardData>>& GetCardDataMap() {
		return CardDataMap;
	}
	void AddPurchaseCard(std::unique_ptr<Card>& card) {
		Card* newCard = card.get();
		allCards.push_back(std::move(card));
		MoveCard(newCard, CardZone::Cemetery);
		
	}

private:
	void CardDraw();
	void StartTurn(TurnState& trunState);

	void MainTurn(TurnState& trunState);

	void EndTurn(TurnState& trunState);
	void CardMoveUpdate();

	void FieldCardEffectCheck(BuildingActivationTiming buildingActivationTiming);

	void OpenDeckAdjustment();

	

	void ReShuffleDeck();

	void ExecutionCard();
public:

	void HandAdjustment();

	std::vector<Card*> GetZoneCards(CardZone zone) {
		return zoneMap[zone];
	}
	std::vector<std::unique_ptr<Card>>& GetAllCard() {
		return allCards;
	}

	std::vector<Card*> OpenDeck(int num, bool isCommand = false);

	void AddSelectCard(Card* card) {
		selectCards.push_back(card);
	}
	std::vector<Card*> GetSelectCards() const {
		return selectCards;
	}
	void SetMinMaxSelectCard(int min, int max) {
		minSelectCard = min;
		maxSelectCard = max;
	}
	void GetMinMaxSelectCard(int& min, int& max) const {
		min = minSelectCard;
		max = maxSelectCard;
	}

	void SetIsSelectCard(bool isSelect);

	bool IsEndSelect() const {
		return isEndSelect;
	}

	void SetIsEndSelect(bool isEnd) {
		isEndSelect = isEnd;
	}

	bool IsEndSelectButton() const;


	void ShuffleCards(std::vector<Card*>& cards);

	void MoveCard(Card* card, CardZone cardZone);

	UIManager* GetUIManager() const {
		return uiManager_;
	}
	ResourceManager* GetResourceManager() const {
		return resourceManager_;
	}

	bool IsMoveCard() const {
		return !cardMoves.empty();
	}
public:
	void AllCardLoad(const std::string& file);

	bool IsHoldCard() const {
		return isHoldCard;
	}
	void SetIsHoldCard(bool isHold) {
		isHoldCard = isHold;
		if (!isHold) {
			holdCardIndex = -1;
		}
	}
	int GetHoldCardIndex() const {
		return holdCardIndex;
	}
	void SetHoldCard(int index) {
		isHoldCard = true;
		holdCardIndex = index;
		effectTextCard_ = zoneMap[CardZone::Hand][holdCardIndex];
	}
	void SetEffectTextCard(Card* card);
	bool IsSelectCard() const {
		return isSelectCard;
	}
	void SetEffectStandby(Card* card) {
		effectStandby_.push(card);
	}
	int HandCardCollision(Vector2 pos);
	void SetFieldCardIndex(Card* card, int index) {
		fieldCardIndexMap[card] = index;
	}
private:
	UIManager* uiManager_ = nullptr;
	ResourceManager* resourceManager_ = nullptr;
	// 乱数生成器
	std::mt19937 seed;
	//　カード場
	std::vector<std::unique_ptr<Card>> allCards;
	std::unordered_map<CardZone, std::vector<Card*>> zoneMap{
		{CardZone::Deck, {}},
		{CardZone::Hand, {}},
		{CardZone::Field, {}},
		{CardZone::Cemetery, {}},
		{CardZone::Execution, {}},
		{CardZone::Open, {}}
	};

	std::queue<Card*> effectStandby_;

	// カードデータ
	std::unordered_map<std::string, std::unique_ptr<CardData>> CardDataMap;

	std::unordered_map<Card*, int> fieldCardIndexMap;

	const int maxFieldCard = 5;
private:
	std::vector<std::vector<std::unique_ptr<CardMove>>> cardMoves;


	std::vector<Card*> selectCards;
	bool isSelectCard = false;
	bool isEndSelect = false;

	int minSelectCard = 0;
	int maxSelectCard = 0;

	int turnCount = 0;

	// 効果テキスト用のカード参照
	Card* effectTextCard_ = nullptr; 

	bool isMove = false;

	// ターン管理
	std::unordered_map<TurnState, std::function<void(TurnState&)>> turnMap{
		{TurnState::Start, [&](TurnState& i) { return StartTurn(i); }},
		{TurnState::Main,  [&](TurnState& i) { return MainTurn(i);  }},
		{TurnState::End,   [&](TurnState& i) { return EndTurn(i);   }},
	};
	bool isStartOpen = true;
	bool isEndStartTurn = false;

	// 開始時のオープンカードの最大枚数
	const int startMaxOpenCard = 5;
	// 現在オープンしたカードの枚数
	int nowOpenCard = 0;

	// 手札調整用
	bool isHoldCard = false;
	int holdCardIndex = -1;

	// サウンド
	int shuffleSE = -1;

	bool isEndStart = false;
};

