#include "ShopManager.h"
#include "CardManager.h"

void ShopManager::Initialize(CardManager* cardManager) {
	std::random_device rd; // 乱数の種
	seed.seed(rd());
	cardManager_ = cardManager;
}

void ShopManager::LoadShopCardData() {
	std::unordered_map<std::string, std::unique_ptr<CardData>>& cardDataMap = cardManager_->GetCardDataMap();
	int id = 1;
	for (const auto& [name, data] : cardDataMap) {
		std::unique_ptr<Card> card(new Card());
		card->InitializeCard(data.get());
		cardMap[id] = data.get();
		CardRarity rarity = card->GetRarity();
		if (rarity == CardRarity::Common) {
			commonCardIDs.push_back(id);
		} else if (rarity == CardRarity::Uncommon) {
			uncommonCardIDs.push_back(id);
		} else if (rarity == CardRarity::Epic) {
			epicCardIDs.push_back(id);
		} else if (rarity == CardRarity::Legendary) {
			legendaryCardIDs.push_back(id);
		}
		id++;
	}
}

void ShopManager::GenerateShopCards() {
	shopCards.clear();
	std::vector<int> generatedCardIDs;
	for (int i = 0; i < kShopCardCount; i++) {
		int rarityRoll = std::uniform_int_distribution<int>(1, kShopTotalRate)(seed);
		int cardID = -1;
		int totalBonus = shopEpicRateBonus_ + shopLegendaaryRateBonus_;
		if (rarityRoll <= kShopRerityCommonBaseRate - totalBonus) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(commonCardIDs, generatedCardIDs));
			continue;
		}
		rarityRoll -= (kShopRerityCommonBaseRate - totalBonus);
		if (rarityRoll <= kShopRerityUncommonBaseRate) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(uncommonCardIDs, generatedCardIDs));
			continue;
		}
		rarityRoll -= kShopRerityUncommonBaseRate;
		if (rarityRoll <= kShopRerityEpicBaseRate + kShopEpicRateBonus) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(epicCardIDs, generatedCardIDs));
			continue;
		}
		generatedCardIDs.push_back(GetUniqueRandomCardID(legendaryCardIDs, generatedCardIDs));
	}
	for (const int& id : generatedCardIDs) {
		GenerateCard(id);
	}
}

int ShopManager::GetUniqueRandomCardID(const std::vector<int>& candidateIDs, const std::vector<int>& generatedCardIDs) {
	std::vector<int> tempGeneratedCardIDs;
	for (const int& id : candidateIDs) {
		bool isUnique = true;
		for (int generatedCardID : generatedCardIDs) {
			if (id == generatedCardID) {
				isUnique = false;
			}
		}
		if (isUnique) {
			tempGeneratedCardIDs.push_back(id);
		}
	}
	
	shopEpicRateBonus_ += kShopEpicRateBonus;
	shopLegendaaryRateBonus_ += kShopLegendaaryRateBonus;

	return GetRandomCardIDByRarity(tempGeneratedCardIDs);
}

void ShopManager::GenerateCard(int cardID) {
	if(cardMap.contains(cardID)) {
		std::unique_ptr<Card> card(new Card());
		card->InitializeCard(cardMap[cardID]);
		shopCards.push_back(std::move(card));
	}
}

int ShopManager::GetRandomCardIDByRarity(const std::vector<int>& cardIDs) {
	if (cardIDs.empty()) {
		return -1;
	}
	std::uniform_int_distribution<int> dist(0, static_cast<int>(cardIDs.size()) - 1);
	int randomIndex = dist(seed);
	return cardIDs[randomIndex];
}
