#include "ShopManager.h"
#include "CardManager.h"
#include "UIManager.h"

#include "SpriteCommon.h"
#include "TextCommon.h"

void ShopManager::Initialize(CardManager* cardManager, UIManager* uiManager) {
	std::random_device rd; // 乱数の種
	seed.seed(rd());
	cardManager_ = cardManager;
	uiManager_ = uiManager;
	shopBackgroundSprite_ = std::make_unique<Sprite>();
	shopBackgroundSprite_->Initialize("white.png");
	shopBackgroundSprite_->SetSize({800.0f, 600.0f});
	shopBackgroundSprite_->SetPosition({640.0f, 360.0f});
	shopBackgroundSprite_->SetColor({0.8f, 0.8f, 0.8f, 1.0f});
	shopBackgroundSprite_->SetAnchorPoint({0.5f, 0.5f});

	LoadShopCardData();
}

void ShopManager::Update(TurnState& turnState) {
	// ショップカードの更新処理
	if(!isStartShop_) {
		uiManager_->SetRerollShopButtonIsDraw(true);
		uiManager_->SetEndShopButtonIsDraw(true);
		GenerateShopCards();
		isStartShop_ = true;
	}
	for (SalesCardData& card : shopCards) {
		card.card->SetIsDraw(true);
		card.card->Update();
		card.priceText->Update();
	}
	shopBackgroundSprite_->Updata();
	if(isEndShop_) {
		uiManager_->SetRerollShopButtonIsDraw(false);
		uiManager_->SetEndShopButtonIsDraw(false);
		isEndShop_ = false;
		isStartShop_ = false;
		turnState = TurnState::Start;
	}
}

void ShopManager::Draw() {
	// ショップカードの描画処理
	SpriteCommon::GetInstance()->PreDraw();
	shopBackgroundSprite_->Draw();
	for (SalesCardData& card : shopCards) {
		SpriteCommon::GetInstance()->PreDraw();
		card.card->Draw();
		TextCommon::GetInstance()->PreDraw();
		card.card->TextDraw();
		card.priceText->Draw();
		TextCommon::GetInstance()->PostDraw();
	}
}

int ShopManager::ShopCardCollision(Vector2 mousePos) {
	int index = 0;
	for (SalesCardData& card : shopCards) {
		if(card.card->IsOnCollision(mousePos)) {
			return index;
		}
		index++;
	}
	return -1;
}

int ShopManager::PurchaseCard(Vector2 mousePos, int money) {
	int index = ShopCardCollision(mousePos);
	if(index == -1) {
		return -1;
	}
	SalesCardData& salesCard = shopCards[index];

	int price = salesCard.price;
	// 足りてるなら購入
	if(money >= price) {
		salesCard.card->SetIsDraw(false);
		cardManager_->AddPurchaseCard(salesCard.card);
		shopCards.erase(shopCards.begin() + index);
		return price;
	}
	return -1;
}

void ShopManager::RerollShopCards() {
	GenerateShopCards();
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

		CardRarity rarity = card->GetRarity();

		if(rarity == CardRarity::Error) {
			continue; // レアリティが不明なカードはスキップ
		}
		if(rarity == CardRarity::Standard) {
			continue; // スタンダードカードはスキップ
		}

		cardMap[id] = data.get();
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
	int i = 0;
	for (const int& id : generatedCardIDs) {
		SalesCardData salesCardData;
		salesCardData.card = GenerateCard(id);
		CardRarity rarity = salesCardData.card->GetRarity();
		if (rarity == CardRarity::Common) {
			salesCardData.price = kShopCommonBasePrice;
		} else if (rarity == CardRarity::Uncommon) {
			salesCardData.price = kShopUncommonBasePrice;
		} else if (rarity == CardRarity::Epic) {
			salesCardData.price = kShopEpicBasePrice;
		} else if (rarity == CardRarity::Legendary) {
			salesCardData.price = kShopLegendaryBasePrice;
		}
		// 価格の振れ幅倍率を計算
		int fluctuation = std::uniform_int_distribution<int>(-kShopPriceFluctuationNormalizationMax, kShopPriceFluctuationNormalizationMax)(seed);
		float t = static_cast<float>(fluctuation) / static_cast<float>(kShopPriceFluctuationNormalizationMax);
		t = t * t * t;
		salesCardData.price += static_cast<int>(salesCardData.price * t * kShopPriceFluctuation);
		// 割引発生判定
		int discountRoll = std::uniform_int_distribution<int>(1, 100)(seed);
		salesCardData.isDiscounted = false;
		if (discountRoll <= kShopDiscountRate) {
			// 割引適用
			salesCardData.price -= static_cast<int>(salesCardData.price * (static_cast<float>(kShopDiscountPercent) / 100.0f));
			salesCardData.isDiscounted = true;
		}
		Vector2 shopCardPos = {640.0f - (static_cast<float>(kShopCardCount-1) / 2.0f - i) * (120.0f + 10.0f), 360.0f};
		salesCardData.card->SetPos(shopCardPos);
		salesCardData.priceText = std::make_unique<Text>();
		std::u32string priceStr = Text::GetIntToString(salesCardData.price, 0);
		if (salesCardData.isDiscounted) {
			std::u32string t = priceStr;
			priceStr = U"<color=rgba(0.3f,0.3f,1.0f,1.0f)>";
			priceStr += t;
		}
		salesCardData.priceText->Initialize(priceStr, {shopCardPos.x, shopCardPos.y + 100.0f}, 2000.0f);
		salesCardData.priceText->SetAnchorPoint({0.5f, 0.0f});
		salesCardData.priceText->Update();
		salesCardData.priceText->CalcFitSizeHeight(40.0f);
		shopCards.push_back(std::move(salesCardData));
		i++;
	}
}

int ShopManager::GetUniqueRandomCardID(const std::vector<int>& candidateIDs, const std::vector<int>& generatedCardIDs) {
	// すでに選んだカードIDを除外した一時的な候補リストを作成
	std::vector<int> tempGeneratedCardIDs;
	for (const int& id : candidateIDs) {
		bool isUnique = true;
		for (int generatedCardID : generatedCardIDs) {
			if (id == generatedCardID) {
				isUnique = true;
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

std::unique_ptr<Card> ShopManager::GenerateCard(int cardID) {
	if(cardMap.contains(cardID)) {
		std::unique_ptr<Card> card(new Card());
		card->InitializeCard(cardMap[cardID]);
		return std::move(card);
	}
	return nullptr;
}

int ShopManager::GetRandomCardID(const std::vector<int>& cardIDs) {
	if (cardIDs.empty()) {
		return -1;
	}
	std::uniform_int_distribution<int> dist(0, static_cast<int>(cardIDs.size()) - 1);
	int randomIndex = dist(seed);
	return cardIDs[randomIndex];
}
