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
struct FieldCard {
	Card* card = nullptr;
	std::unique_ptr<Button> field;
	bool isOn = false;
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

	std::vector<Card*> OpenDeck(int num, bool isCommand = false);

	void AddSelectCard(Card* card) {
		selectCards.push_back(card);
	}

	void SetIsSelectCard(bool isSelect);

	void SetEndSelectButtonColorV(float v);

	bool IsEndSelectButton() const;


	void ShufleCards(std::vector<Card*>& cards);

	void MoveCard(Card* card, CardZone cardZone);

	bool IsMoveCard() const {
		return !cardMoves.empty();
	}

	void CostTextUpdate();
public:
	void AllCardLoad(const std::string& file);

	Vector2 GetCardPos(CardZone zone, int index);
private:
	Vector2 HandCardPos(int index);
	Vector2 OpenCardPos(int index);
	Vector2 CemeteryCardPos();
	Vector2 FieldCardPos(int index);
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

	std::queue<Card*> effectStandby_;

	// カードデータ
	std::unordered_map<std::string, std::unique_ptr<CardData>> CardDataMap;

	const int maxFieldCard = 5;
private:
	std::vector<std::vector<std::unique_ptr<CardMove>>> cardMoves;

	// ボタン
	std::unique_ptr<Button> endTurnButton = nullptr;

	std::unique_ptr<Button> startOpenButton = nullptr;
	std::unique_ptr<Button> startOpenEndButton = nullptr;

	std::unique_ptr<Button> endSelectButton = nullptr;
	const Vector4 endSelectButtonColor = {0.0f, 0.5f, 1.0f, 1.0f};

	//
	std::unique_ptr<Button> cardExecutionField = nullptr;
	std::vector<FieldCard> fieldCardField;


	// コスト表示用背景スプライト
	std::unique_ptr<Sprite> costBackSprite = nullptr;
	std::unique_ptr<Sprite> costBackSprite2 = nullptr;

	// コスト表示用テキスト
	std::unique_ptr<Text> lightCostText = nullptr;
	std::unique_ptr<Text> darknessCostText = nullptr;

	// エネルギーコスト
	int lightCost = 0;
	int darknessCost = 0;

	// 効果テキスト用のカード参照
	Card* effectTextCard_ = nullptr; 

	// カード選択中かどうか
	bool isSelectCard = false;
	std::unique_ptr<Sprite> selectCardBackSprite = nullptr;

	std::vector<Card*> selectCards;

	float selectCardBackSpriteAlpha = 0.0f;

	const float selectCardBackSpriteMaxAlpha = 0.5f;

	bool isMove = false;

	// ターン管理
	std::unordered_map<TrunState, std::function<void(TrunState&)>> trunMap{
		{TrunState::Start, [&](TrunState& i) {return StartTrun(i); }},
		{TrunState::Main,  [&](TrunState& i) {return MainTrun(i);  }},
		{TrunState::End,   [&](TrunState& i) {return EndTrun(i);   }},
	};
	bool isStartOpen = true;
	bool isEndStartTrun = false;

	float cardSizeW = 120.0f;

	// カード間のパディング
	const float openCardPading = 10.0f;
	const float handCardPading = 5.0f;
	const float fieldCardPading = 20.0f;

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

