#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <memory>

#include "TrunState.h"

#include "Sprite.h"
#include "Text.h"

class CardManager;
class UIManager;
class CardData;
class Card;

struct SalesCardData {
	std::unique_ptr<Card> card;
	std::unique_ptr<Text> priceText;
	int price;
	bool isDiscounted;
};

class ShopManager {
public:
	void Initialize(CardManager* cardManager, UIManager* uiManager);

	void Update(TurnState& turnState);

	void Draw();

	int ShopCardCollision(Vector2 mousePos);

	int PurchaseCard(Vector2 mousePos, int money);

	void RerollShopCards();

	/// <summary>
	/// ショップカードデータの読み込み
	/// </summary>
	void LoadShopCardData();

	void SetIsEndShop(bool isEnd) {
		isEndShop_ = isEnd;
	}

private:
	/// <summary>
	/// ショップカードの生成
	/// </summary>
	void GenerateShopCards();

	/// <summary>
	/// ユニークなランダムカードIDの取得
	/// </summary>
	/// <param name="candidateIDs">ランダムに選ぶカードIDs</param>
	/// <param name="generatedCardIDs">すでに選んだカードIDs</param>
	/// <returns>カードID</returns>
	int GetUniqueRandomCardID(const std::vector<int>& candidateIDs, const std::vector<int>& generatedCardIDs);

	/// <summary>
	/// カードの生成
	/// </summary>
	/// <param name="cardID">生成するカードID</param>
	std::unique_ptr<Card> GenerateCard(int cardID);

	/// <summary>
	/// ランダムなカードIDの取得
	/// </summary>
	/// <param name="cardIDs">ランダムに選ぶカードIDs</param>
	/// <returns>カードID</returns>
	int GetRandomCardID(const std::vector<int>& cardIDs);

private:
	std::mt19937 seed;
	CardManager* cardManager_ = nullptr;
	UIManager* uiManager_ = nullptr;
	std::unordered_map<int, CardData*> cardMap;
	std::vector<int> commonCardIDs;
	std::vector<int> uncommonCardIDs;
	std::vector<int> epicCardIDs;
	std::vector<int> legendaryCardIDs;

	bool isStartShop_ = false;
	bool isEndShop_ = false;

	// 販売カード数
	const int kShopCardCount = 5;

	// 初期確率
	const int kShopRarityCommonBaseRate = 650;
	const int kShopRarityUncommonBaseRate = 300;
	const int kShopRarityEpicBaseRate = 50;
	const int kShopRarityLegendaryBaseRate = 0;
	const int kShopTotalRate = kShopRarityCommonBaseRate + kShopRarityUncommonBaseRate + kShopRarityEpicBaseRate + kShopRarityLegendaryBaseRate;

	// 一回のカード生成で上昇する確率ボーナス
	const int kShopEpicRateBonus = 4;
	const int kShopLegendaryRateBonus = 1;

	// 確率ボーナス
	int shopEpicRateBonus_ = 0;
	int shopLegendaryRateBonus_ = 0;

	// レア度別基本価格
	const int kShopCommonBasePrice = 50;
	const int kShopUncommonBasePrice = 100;
	const int kShopEpicBasePrice = 300;
	const int kShopLegendaryBasePrice = 600;

	// 価格振れ幅倍率(%)
	const float kShopPriceFluctuation = 0.2f;

	// 価格振れ幅倍率にイージングによる重みを付与するための最大値
	const int kShopPriceFluctuationNormalizationMax = 100;

	// 割引発生確率
	const int kShopDiscountRate = 10;

	// 割引率(%)
	const int kShopDiscountPercent = 50;

	// ショップで販売されているカードリスト
	std::vector<SalesCardData> shopCards;

	// ショップ背景スプライト
	std::unique_ptr<Sprite> shopBackgroundSprite_ = nullptr;


};

