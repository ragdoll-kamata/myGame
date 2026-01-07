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

	void LoadShopCardData();


private:
	void GenerateShopCards();
	int GetUniqueRandomCardID(const std::vector<int>& candidateIDs, const std::vector<int>& generatedCardIDs);
	void GenerateCard(int cardID);
	int GetRandomCardIDByRarity(const std::vector<int>& cardIDs);

private:
	std::mt19937 seed;
	CardManager* cardManager_ = nullptr;
	std::unordered_map<int, CardData*> cardMap;
	std::vector<int> commonCardIDs;
	std::vector<int> uncommonCardIDs;
	std::vector<int> epicCardIDs;
	std::vector<int> legendaryCardIDs;

	std::vector<std::unique_ptr<Card>> shopCards;

	const int kShopCardCount = 5;
	const int kShopRerityCommonBaseRate = 650;
	const int kShopRerityUncommonBaseRate = 300;
	const int kShopRerityEpicBaseRate = 50;
	const int kShopRerityLegendaaryBaseRate = 0;
	const int kShopTotalRate = kShopRerityCommonBaseRate + kShopRerityUncommonBaseRate + kShopRerityEpicBaseRate + kShopRerityLegendaaryBaseRate;

	const int kShopEpicRateBonus = 4;
	const int kShopLegendaaryRateBonus = 1;

	int shopEpicRateBonus_ = 0;
	int shopLegendaaryRateBonus_ = 0;

};

