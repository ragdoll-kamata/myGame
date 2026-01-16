#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <memory>
class CardManager;
class CardData;
class Card;
class ShopManager {
public:
	void Initialize(CardManager* cardManager);

	void Update();

	void Draw();

	/// <summary>
	/// ショップカードデータの読み込み
	/// </summary>
	void LoadShopCardData();


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
	void GenerateCard(int cardID);

	/// <summary>
	/// ランダムなカードIDの取得
	/// </summary>
	/// <param name="cardIDs">ランダムに選ぶカードIDs</param>
	/// <returns>カードID</returns>
	int GetRandomCardID(const std::vector<int>& cardIDs);

private:
	std::mt19937 seed;
	CardManager* cardManager_ = nullptr;
	std::unordered_map<int, CardData*> cardMap;
	std::vector<int> commonCardIDs;
	std::vector<int> uncommonCardIDs;
	std::vector<int> epicCardIDs;
	std::vector<int> legendaryCardIDs;

	std::vector<std::unique_ptr<Card>> shopCards;

	// 初期確率
	const int kShopCardCount = 5;
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

};

