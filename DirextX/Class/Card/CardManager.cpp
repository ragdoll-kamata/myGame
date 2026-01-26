#include "CardManager.h"
#include <iostream>
#include <filesystem>
#include <vector>

#include <fstream>
#include <sstream>

#include <Input.h>

#include "TextCommon.h"
#include "SpriteCommon.h"

#include "HandCardMove.h"
#include "CardShuffleMove.h"
#include "OpenDeckAlignmentMove.h"

#include "Audio.h"

#include "UIManager.h"
#include "ResourceManager.h"

namespace fs = std::filesystem;

void CardManager::Initialize(UIManager* uiManager, ResourceManager* resourceManager) {
	uiManager_ = uiManager;
	resourceManager_ = resourceManager;
	std::random_device rd; // 乱数の種
	seed.seed(rd());
	shuffleSE = Audio::GetInstance()->LoadSound("cardShuffle.mp3");
	turnCount = 1;

	effectTextCardBackSprite_ = std::make_unique<Button>();
	effectTextCardBackSprite_->Initialize({},{1.0f,1.0f}, "white.png", {0.5f, 0.5f, 0.5f, 1.0f});
	effectTextCardBackSprite_->SetAnchorPoint({0.0f, 0.0f});
	effectTextCardBackSprite_->SetPosition({0.0f, 0.0f});
	effectTextCardBackSprite_->SetColor({0.5f, 0.5f, 0.5f, 1.0f});
	effectTextCardBackSprite_->SetIsDraw(true);
}

bool CardManager::StartCardSet() {
	std::ifstream file("StartCard.txt");
	if (!file) {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return false;
	}

	std::string line;
	int lineNumber = 0;
	while (std::getline(file, line)) {
		std::string name;
		std::string num;
		bool is = false;
		for (char c : line) {
			if (c == ' ') {
				continue;
			}
			if (c == ',') {
				is = true;
				continue;
			}
			if (!is) {
				name += c;
			} else {
				num += c;
			}

		}

		if (CardDataMap.contains(name)) {
			int number = 0;
			try {
				number = std::stoi(num);
			} catch (const std::invalid_argument& e) {
				e;
				number = 1;
			} catch (const std::out_of_range& e) {
				e;
				number = 1;
			}
			
			for (int i = 0; i < number; i++) {
				std::unique_ptr<Card> card(new Card());
				card->InitializeCard(CardDataMap[name].get());
				zoneMap[CardZone::Deck].push_back(card.get());
				card->SetZone(CardZone::Deck);
				allCards.push_back(std::move(card));
			}
		}
	}

	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), seed);
	return true;
}

void CardManager::Update(TurnState& turnState) {
	if (turnMap.find(turnState) != turnMap.end()) {
		turnMap[turnState](turnState);
	}

	if (!isMove) {
		ExecutionCard();
	}

	if (effectStandby_.empty()) {
		for (auto& card : GetZoneCards(CardZone::Field)) {
			if (card->GetDurability() <= 0) {
				std::unique_ptr<CardMove> moveCard = std::make_unique<CardMove>();
				moveCard->Initialize(card, uiManager_->GetCardPos(CardZone::Cemetery, 0, 0), 0.5f, false);
				std::vector<std::unique_ptr<CardMove>> moves;
				moves.push_back(std::move(moveCard));
				AddCardMove(std::move(moves));

				MoveCard(card, CardZone::Cemetery);

				int index = fieldCardIndexMap[card];
				uiManager_->SetFieldCardFieldIsOn(index, false);
				fieldCardIndexMap.erase(card);
			}
		}
	}

	CardMoveUpdate();

	for (const auto& card : allCards) {
		card->Update();
	}
}

void CardManager::Draw() {
	TextCommon::GetInstance()->PreDraw();

	CardDraw();
}

void CardManager::SelectDraw() {
	if (isSelectCard) {
		for (const auto& card : selectCards) {
			SpriteCommon::GetInstance()->PreDraw();
			card->Draw();

			TextCommon::GetInstance()->PreDraw();
			card->TextDraw();
			TextCommon::GetInstance()->PostDraw();
		}
	}
}

void CardManager::TextDraw() {
	for (const auto& card : allCards) {
		card->TextDraw();
	}

}
void CardManager::EffectTextDraw() {
	if (effectTextCard_) {
		SpriteCommon::GetInstance()->PreDraw();
		effectTextCardBackSprite_->Draw();
		TextCommon::GetInstance()->PreDraw();
		effectTextCard_->EffectTextDraw();
		TextCommon::GetInstance()->PostDraw();
	}
}

void CardManager::AddCardMove(std::vector<std::unique_ptr<CardMove>> moveCard) {
	cardMoves.push_back(std::move(moveCard));
}

void CardManager::CardDraw() {
	TextCommon::GetInstance()->PostDraw();
	for (const auto& card : allCards) {
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}

	for (auto* card : zoneMap[CardZone::Hand]) {
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}
	if (isHoldCard) {
		Card* card = zoneMap[CardZone::Hand][holdCardIndex];
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}
}

void CardManager::StartTurn(TurnState& turnState) {
	Input* input = Input::GetInstance();
	uiManager_->SetStartOpenButtonIsDraw(true);
	uiManager_->SetStartOpenEndButtonIsDraw(true);
	if (!isEndStartTurn) {
		if (isStartOpen) {
			OpenDeck(1);
			isStartOpen = false;
			nowOpenCard++;
			if (nowOpenCard >= startMaxOpenCard) {
				isEndStartTurn = true;
			}
		}
		Vector2 mousePos = input->GetMousePos();
		if (input->TriggerMouseButton(0)) {
			if (uiManager_->IsOnCollisionStartOpenButton(mousePos) && nowOpenCard < startMaxOpenCard) {
				isStartOpen = true;
			}
			if (uiManager_->IsOnCollisionStartOpenEndButton(mousePos)) {
				isEndStartTurn = true;
			}
		}
	} else {
		uiManager_->SetStartOpenButtonIsDraw(false);
		uiManager_->SetStartOpenEndButtonIsDraw(false);
		bool is = false;
		for (const auto& card : zoneMap[CardZone::Open]) {
			if (card->IsMove()) {
				is = true;
				break;
			}
		}
		if (IsMoveCard()) {
			is = true;
		}
		if (!is) {
			isEndStartTurn = false;
			isStartOpen = true;
			nowOpenCard = 0;
			OpenDeckAdjustment();
			turnState = TurnState::Main;
		}
		//

	}


}

void CardManager::MainTurn(TurnState& turnState) {
	uiManager_->SetEndTurnButtonIsDraw(true);
	Input* input = Input::GetInstance();
	Vector2 mousePos = input->GetMousePos();
	if (input->TriggerMouseButton(0)) {
		if (uiManager_->IsOnCollisionEndTurnButton(mousePos)) {
			FieldCardEffectCheck(BuildingActivationTiming::EndTurn);
			uiManager_->SetEndTurnButtonIsDraw(false);
			resourceManager_->AddRunawayGauge(10.0f);
			turnState = TurnState::End;
		}
	}
}

void CardManager::EndTurn(TurnState& turnState) {
	isHoldCard = false;
	if (!effectStandby_.empty()) {
		return;
	}
	
	if (!isEndStart) {
		std::vector<Card*> handCards = zoneMap[CardZone::Hand];
		std::vector<std::unique_ptr<CardMove>> moves;
		for (auto& card : handCards) {
			if (card->GetIsCardCharacteristics(CardCharacteristics::Protection)) {
				continue;
			}
			if (card->GetIsCardCharacteristics(CardCharacteristics::TemporaryProtection)) {
				card->SetIsCardCharacteristics(CardCharacteristics::TemporaryProtection, false);
				continue;
			}
			std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
			move->Initialize(card, uiManager_->GetCardPos(CardZone::Cemetery, 0, 0), 0.5f, false);
			moves.push_back(std::move(move));
			MoveCard(card, CardZone::Cemetery);
		}
		if (!moves.empty()) {
			AddCardMove(std::move(moves));
		}
		if (!zoneMap[CardZone::Hand].empty()) {
			std::unique_ptr<HandCardMove> move = std::make_unique<HandCardMove>();
			move->Initialize(zoneMap[CardZone::Hand], 0.5f);
			std::vector<std::unique_ptr<CardMove>> moveVec;
			moveVec.push_back(std::move(move));
			AddCardMove(std::move(moveVec));
		}
		isEndStart = true;
	}
	if (IsMoveCard()) {
		return;
	}
	
	turnCount++;
	
	isEndStart = false;
	turnState = TurnState::Start;
	if (turnCount % 3 == 0) {
		turnState = TurnState::Shop;
	}
}

void CardManager::CardMoveUpdate() {
	isMove = false;
	if (IsMoveCard()) {
		isMove = true;
		bool isEnd = true;
		for (const auto& card : cardMoves[0]) {
			card->Update();
			if (!card->IsEnd()) {
				isEnd = false;
			}
		}
		if (isEnd) {
			for (const auto& card : cardMoves[0]) {
				card->End();
			}
			cardMoves.front().clear();
			cardMoves.erase(cardMoves.begin());
			if (IsMoveCard()) {
				for (const auto& card : cardMoves[0]) {
					card->SetStart();
				}
			}
		}
	}
}

void CardManager::FieldCardEffectCheck(BuildingActivationTiming buildingActivationTiming) {
	for (const auto& card : zoneMap[CardZone::Field]) {
		if (card->GetBuildingActivationTiming() == buildingActivationTiming) {
			effectStandby_.push(card);
		}
	}
}

void CardManager::OpenDeckAdjustment() {

	int lIndex = 0;
	int dIndex = 0;
	std::vector<Card*> LightCards;
	std::vector<Card*> DarknessCards;
	std::vector<Card*> NoneCards;

	// 光と闇の数を数える
	for (const auto& card : zoneMap[CardZone::Open]) {
		if (card->GetElement() == CardElement::Light) {
			lIndex++;
			LightCards.push_back(card);
		}
		if (card->GetElement() == CardElement::Darkness) {
			dIndex++;
			DarknessCards.push_back(card);
		}
		if (card->GetElement() == CardElement::None) {
			NoneCards.push_back(card);
		}
	}
	std::list<Card*> removeCards;
	std::list<Card*> addCards;

	//// 手札に加えるカードと墓地に送るカードを決定
	// 無属性は必ず手札に加える
	addCards.insert(addCards.end(), NoneCards.begin(), NoneCards.end());

	//// 光と闇の数で場合分け
	if (lIndex > dIndex) {
		// 光の方が多いなら、闇を墓地に送る
		removeCards.insert(removeCards.end(), DarknessCards.begin(), DarknessCards.end());
		addCards.insert(addCards.end(), LightCards.begin(), LightCards.end());
	} else if (lIndex < dIndex) {
		// 闇の方が多いなら、光を墓地に送る
		removeCards.insert(removeCards.end(), LightCards.begin(), LightCards.end());
		addCards.insert(addCards.end(), DarknessCards.begin(), DarknessCards.end());
	} else {
		// 同じなら両方手札に加える
		addCards.insert(addCards.end(), LightCards.begin(), LightCards.end());
		addCards.insert(addCards.end(), DarknessCards.begin(), DarknessCards.end());
	}
	std::list<Card*> openCards;
	openCards.insert(openCards.end(), zoneMap[CardZone::Open].begin(), zoneMap[CardZone::Open].end());

	//// 実際にカードを移動

	// 手札に加えるカードを移動
	for (const auto& card : openCards) {
		for (const auto& card2 : addCards) {
			if (card == card2) {
				MoveCard(card, CardZone::Hand);
				break;
			}
		}
	}
	// 手札に加えるカードの移動アニメーション
	std::vector<std::unique_ptr<CardMove>> moves;
	std::unique_ptr<HandCardMove> handMove = std::make_unique<HandCardMove>();
	std::vector<Card*> handCards;
	for (const auto& card : addCards) {
		handCards.push_back(card);
	}
	handMove->Initialize(handCards, 0.3f);
	moves.push_back(std::move(handMove));
	AddCardMove(std::move(moves));

	moves.clear();
	// 墓地に送るカードを移動
	for (const auto& card : removeCards) {
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(card, uiManager_->GetCardPos(CardZone::Cemetery, 0, 0), 0.5f, false);
		moves.push_back(std::move(move));
		MoveCard(card, CardZone::Cemetery);
	}
	AddCardMove(std::move(moves));
}

void CardManager::HandAdjustment() {
	Vector2 pos;
	int i = 0;
	int size = static_cast<int>(zoneMap[CardZone::Hand].size()) - 1;
	for (const auto& card : zoneMap[CardZone::Hand]) {
		if (card->IsCommandMove()) {
			size--;
		}
	}
	for (const auto& card : zoneMap[CardZone::Hand]) {
		if (card->IsCommandMove()) {
			continue;
		}
		pos = uiManager_->GetCardPos(CardZone::Hand, i, size);
		card->SetNewPos(pos);
		card->SetIsDraw(true);
		i++;
	}
}

void CardManager::ReShuffleDeck() {
	std::vector<Card*> cemeteryCards = zoneMap[CardZone::Cemetery];
	Vector2 pos = Vector2{-100.0, -100.0f};
	resourceManager_->AddRunawayGauge(5.0f);
	for (const auto& card : cemeteryCards) {
		card->SetPos(pos);
		card->RessetVariable();
		card->SetIsDraw(false);
		MoveCard(card, CardZone::Deck);
	}
	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), seed);

	std::unique_ptr<CardShuffleMove> shuffleMove = std::make_unique<CardShuffleMove>();
	shuffleMove->Initialize(cemeteryCards, shuffleSE, 1.0f);
	std::vector<std::unique_ptr<CardMove>> moves;
	moves.push_back(std::move(shuffleMove));
	AddCardMove(std::move(moves));

}

void CardManager::ExecutionCard() {
	if (!IsMoveCard()) {
		if (!effectStandby_.empty()) {

			if (effectStandby_.front()->Effect()) {
				if (effectStandby_.front()->GetZone() == CardZone::Execution) {
					MoveCard(effectStandby_.front(), CardZone::Cemetery);
				}
				effectStandby_.pop();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

std::vector<Card*> CardManager::OpenDeck(int num, bool isCommand) {
	std::vector<Card*> result;
	Vector2 pos{640.0f, -160.0f};
	for (int i = 0; i < num; i++) {
		if (zoneMap[CardZone::Deck].empty()) {
			ReShuffleDeck();
		}
		zoneMap[CardZone::Deck].front()->SetPos(pos);
		zoneMap[CardZone::Deck].front()->SetIsDraw(true);
		result.push_back(zoneMap[CardZone::Deck].front());
		if (zoneMap[CardZone::Deck].front()->GetElement() == CardElement::Light) {
			resourceManager_->AddLightCost(1);
		} else if (zoneMap[CardZone::Deck].front()->GetElement() == CardElement::Darkness) {
			resourceManager_->AddDarknessCost(1);
		}
		MoveCard(zoneMap[CardZone::Deck].front(), CardZone::Open);
	}
	if (!isCommand) {
		std::unique_ptr<OpenDeckAlignmentMove> openMove = std::make_unique<OpenDeckAlignmentMove>(); \
			std::vector<std::unique_ptr<CardMove>> moves;
		moves.push_back(std::move(openMove));
		AddCardMove(std::move(moves));
	}
	return result;
}

void CardManager::SetIsSelectCard(bool isSelect) {
	isSelectCard = isSelect;
	uiManager_->SetIsSelectCard(isSelect);
	uiManager_->SetEndSelectButtonIsDraw(isSelect);
	if (!isSelect) {
		selectCards.clear();
		isEndSelect = false;
	}
}

bool CardManager::IsEndSelectButton() const {
	return uiManager_->IsOnCollisionEndSelectButton(Input::GetInstance()->GetMousePos());
}

void CardManager::ShuffleCards(std::vector<Card*>& cards) {
	std::shuffle(cards.begin(), cards.end(), seed);
}

void CardManager::MoveCard(Card* card, CardZone cardZone) {

	std::vector<Card*>& zone = zoneMap[card->GetZone()];
	auto it = std::find(zone.begin(), zone.end(), card);
	if (it != zone.end()) {
		zone.erase(it); // target を vector から削除
	}

	card->SetZone(cardZone);
	zoneMap[cardZone].push_back(card);
}

/////////////////////////////////////////////////////////////////


void CardManager::AllCardLoad(const std::string& file) {
	std::string basePath = file;
	std::vector<CardFile> cardFiles;

	for (const auto& entry : fs::recursive_directory_iterator(basePath)) {
		if (entry.is_regular_file()) {
			auto path = entry.path();
			std::u8string modName8 = path.parent_path().filename().u8string(); // mod名
			std::string modName(modName8.begin(), modName8.end());

			std::u8string fileName8 = path.filename().u8string();
			std::string fileName(fileName8.begin(), fileName8.end());

			cardFiles.push_back({modName, path.string(), fileName});
			std::unique_ptr<CardData> cardData = std::make_unique<CardData>();
			cardData->LoadCardFile(path.string());
			CardDataMap.insert(std::pair(fileName, std::move(cardData)));
		}
	}


}

void CardManager::SetHoldCard(int index) {
	isHoldCard = true;
	holdCardIndex = index;
	effectTextCard_ = zoneMap[CardZone::Hand][holdCardIndex];
	CalcEffectTextBackSpriteSize();
}

void CardManager::SetEffectTextCard(Card* card) {
	effectTextCard_ = card;
	CalcEffectTextBackSpriteSize();
}

void CardManager::SetEffectTextCardPos(Vector2 pos) {
	effectTextCard_->SetEffectTextPos(pos);
	effectTextCardBackSprite_->SetPosition(pos);
}

int CardManager::HandCardCollision(Vector2 pos) {
	int index = 0;
	for (const auto& card : zoneMap[CardZone::Hand]) {
		if (card->IsDraw() && card->IsOnCollision(pos)) {
			return index;
		}
		index++;
	}
	return -1;
}

void CardManager::CalcEffectTextBackSpriteSize() {
	float size = effectTextCard_->GetEffectTextHeight();
	effectTextCardBackSprite_->SetSize({500.0f, size});
}
