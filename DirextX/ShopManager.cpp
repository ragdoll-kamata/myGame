#include "ShopManager.h"
#include "CardManager.h"

void ShopManager::Initialize(CardManager* cardManager) {
	std::random_device rd; // 乱数の種
	seed.seed(rd());
	cardManager_ = cardManager;
}

void ShopManager::Update() {
	// ショップカードの更新処理
}

void ShopManager::Draw() {
	// ショップカードの描画処理
}

void ShopManager::LoadShopCardData() {
	// カードデータの分類
	// カードデータマップの取得
	std::unordered_map<std::string, std::unique_ptr<CardData>>& cardDataMap = cardManager_->GetCardDataMap();
	int id = 1;
	// カードデータをレアリティごとに分類し、カードIDを割り当てる
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
		// ロール
		int rarityRoll = std::uniform_int_distribution<int>(1, kShopTotalRate)(seed);
		int cardID = -1;
		int totalBonus = shopEpicRateBonus_ + shopLegendaryRateBonus_;
		// コモン判定
		if (rarityRoll <= kShopRarityCommonBaseRate - totalBonus) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(commonCardIDs, generatedCardIDs));
			continue;
		}
		// コモン確率減算
		rarityRoll -= (kShopRarityCommonBaseRate - totalBonus);
		// アンコモン判定
		if (rarityRoll <= kShopRarityUncommonBaseRate) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(uncommonCardIDs, generatedCardIDs));
			continue;
		}
		// アンコモン確率減算
		rarityRoll -= kShopRarityUncommonBaseRate;
		// エピック判定
		if (rarityRoll <= kShopRarityEpicBaseRate + shopEpicRateBonus_) {
			generatedCardIDs.push_back(GetUniqueRandomCardID(epicCardIDs, generatedCardIDs));
			shopEpicRateBonus_ = 0;
			continue;
		}
		// 余りの確率はレジェンダリー
		// レジェンダリー判定
		generatedCardIDs.push_back(GetUniqueRandomCardID(legendaryCardIDs, generatedCardIDs));
		shopLegendaryRateBonus_ = 0;
	}
	for (const int& id : generatedCardIDs) {
		GenerateCard(id);
	}
}

int ShopManager::GetUniqueRandomCardID(const std::vector<int>& candidateIDs, const std::vector<int>& generatedCardIDs) {
	// すでに選んだカードIDを除外した一時的な候補リストを作成
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
	
	// 確率ボーナスの更新
	shopEpicRateBonus_ += kShopEpicRateBonus;
	shopLegendaryRateBonus_ += kShopLegendaryRateBonus;

	// ランダムにカードIDを取得
	return GetRandomCardID(tempGeneratedCardIDs);
}

void ShopManager::GenerateCard(int cardID) {
	if(cardMap.contains(cardID)) {
		std::unique_ptr<Card> card(new Card());
		card->InitializeCard(cardMap[cardID]);
		shopCards.push_back(std::move(card));
	}
}

int ShopManager::GetRandomCardID(const std::vector<int>& cardIDs) {
	if (cardIDs.empty()) {
		return -1;
	}
	std::uniform_int_distribution<int> dist(0, static_cast<int>(cardIDs.size()) - 1);
	int randomIndex = dist(seed);
	return cardIDs[randomIndex];
}
