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

class CardManager {
public:
	void Initialize();

	bool StartCardSet();

	void Update(TrunState& trunState);

	void Draw();

	void TextDraw();

	void AddCardMove(std::vector<std::unique_ptr<CardMove>> moveCard);

private:
	void StartTrun(TrunState& trunState);

	void MainTrun(TrunState& trunState);

	void EndTrun(TrunState& trunState);

	void PlayerInput();

	

	void OpenDeckAdjustment();

	

	void ReShuffleDeck();

	void ExecutionCard();
public:
	void HandAdjustment();

	std::vector<Card*> GetZoneCards(CardZone zone) {
		return zoneMap[zone];
	}

	std::vector<Card*> OpenDeck(int num, bool isCommand = false);

	void MoveCard(Card* card, CardZone cardZone);
public:
	void AllCardLoad(const std::string& file);

	Vector2 GetCardPos(CardZone zone, int index);
private:
	Vector2 HandCardPos(int index);
	Vector2 OpenCardPos(int index);
	Vector2 CemeteryCardPos();
private:
	// 乱数生成器
	std::mt19937 g;
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

	// カードデータ
	std::unordered_map<std::string, std::unique_ptr<CardData>> CardDataMap;
private:
	std::vector<std::vector<std::unique_ptr<CardMove>>> cardMoves;

	std::unique_ptr<Button> endTurnButton = nullptr;

	std::unique_ptr<Button> startOpenButton = nullptr;
	std::unique_ptr<Button> startOpenEndButton = nullptr;

	std::unique_ptr<Button> cardExecutionField = nullptr;
	// ターン管理
	std::unordered_map<TrunState, std::function<void(TrunState&)>> trunMap{
		{TrunState::Start, [&](TrunState& i) {return StartTrun(i); }},
		{TrunState::Main,  [&](TrunState& i) {return MainTrun(i);  }},
		{TrunState::End,   [&](TrunState& i) {return EndTrun(i);   }},
	};
	bool isStartOpen = true;
	bool isEndStartTrun = false;

	// 開始時のオープンカードの最大枚数
	const int startMaxOpenCard = 5;
	// 現在オープンしたカードの枚数
	int nowOpenCard = 0;

	// 手札調整用
	bool isHoldCard = false;
	int holdCardIndex = -1;
};

