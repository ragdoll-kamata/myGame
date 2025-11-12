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

#include "Audio.h"

namespace fs = std::filesystem;

void CardManager::Initialize() {
	std::random_device rd; // 乱数の種
	g.seed(rd());
	endTurnButton = std::make_unique<Button>();
	endTurnButton->Initialize({1100.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {1.0f, 0.0f, 0.0f, 1.0f});
	endTurnButton->SetIsDraw(false);

	startOpenButton = std::make_unique<Button>();
	startOpenButton->Initialize({200.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 1.0f, 0.0f, 1.0f});
	startOpenButton->SetIsDraw(false);

	startOpenEndButton = std::make_unique<Button>();
	startOpenEndButton->Initialize({440.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 0.0f, 1.0f, 1.0f});
	startOpenEndButton->SetIsDraw(false);

	cardExecutionField = std::make_unique<Button>();
	cardExecutionField->Initialize({1150.0f, 400.0f}, {120.0f * 1.2f, 160.0f * 1.2f}, "white.png", {0.0f, 1.0f, 1.0f, 1.0f});
	cardExecutionField->SetIsDraw(true);
	shuffleSE = Audio::GetInstance()->LoadSound("cardShuffle.mp3");
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
		if (CardDataMap.contains(line)) {
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<Card> card(new Card());
				card->InitializeCard(CardDataMap[line].get());
				zoneMap[CardZone::Deck].push_back(card.get());
				card->SetZone(CardZone::Deck);
				allCards.push_back(std::move(card));
			}
		}
	}

	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), g);
	return true;
}

void CardManager::Update(TrunState& trunState) {
	trunMap[trunState](trunState);

	for (const auto& card : allCards) {
		card->Update();
	}

	bool isMove = false;
	if (cardMoves.size() > 0) {
		isMove = true;
		bool isEnd = false;
		for (const auto& card : cardMoves[0]) {
			card->Update();
			if (!card->IsEnd()) {
				isEnd = true;
			}
		}
		if (!isEnd) {
			for (const auto& card : cardMoves[0]) {
				card->End();
			}
			cardMoves.front().clear();
			cardMoves.erase(cardMoves.begin());
			if (cardMoves.size() > 0) {
				for (const auto& card : cardMoves[0]) {
					card->SetStart();
				}
			}
		}
	}
	if (!isMove) {
		ExecutionCard();
	}



	endTurnButton->Update();
	startOpenButton->Update();
	startOpenEndButton->Update();
	cardExecutionField->Update();
}

void CardManager::Draw() {

	SpriteCommon::GetInstance()->PreDraw();
	endTurnButton->Draw();
	startOpenButton->Draw();
	startOpenEndButton->Draw();
	endTurnButton->Draw();
	cardExecutionField->Draw();
	for (const auto& card : allCards) {
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();
		TextCommon::GetInstance()->PreDraw();

		card->TextDraw();

		TextCommon::GetInstance()->PostDraw();

	}
}

void CardManager::TextDraw() {
	for (const auto& card : allCards) {
		card->TextDraw();
	}

}

void CardManager::AddCardMove(std::vector<std::unique_ptr<CardMove>> moveCard) {
	cardMoves.push_back(std::move(moveCard));
}

void CardManager::StartTrun(TrunState& trunState) {
	Input* input = Input::GetInstance();
	startOpenButton->SetIsDraw(true);
	startOpenEndButton->SetIsDraw(true);
	if (!isEndStartTrun) {
		if (isStartOpen) {
			OpenDeck(1);
			isStartOpen = false;
			nowOpenCard++;
			if (nowOpenCard >= startMaxOpenCard) {
				isEndStartTrun = true;
			}
		}
		Vector2 mousePos = input->GetMousePos();
		if (input->TriggerMouseButton(0)) {
			if (startOpenButton->IsOnCollision(mousePos) && nowOpenCard < startMaxOpenCard) {
				isStartOpen = true;
			}
			if (startOpenEndButton->IsOnCollision(mousePos)) {
				isEndStartTrun = true;
			}
		}
	} else {
		startOpenButton->SetIsDraw(false);
		startOpenEndButton->SetIsDraw(false);
		bool is = false;
		for (const auto& card : zoneMap[CardZone::Open]) {
			if (card->IsMove()) {
				is = true;
				break;
			}
		}
		if (!is) {
			isEndStartTrun = false;
			isStartOpen = true;
			nowOpenCard = 0;
			OpenDeckAdjustment();
			trunState = TrunState::Main;
		}
		//

	}


}

void CardManager::MainTrun(TrunState& trunState) {
	endTurnButton->SetIsDraw(true);
	Input* input = Input::GetInstance();
	Vector2 mousePos = input->GetMousePos();
	PlayerInput();
	if (input->TriggerMouseButton(0)) {
		if (endTurnButton->IsOnCollision(mousePos)) {
			endTurnButton->SetIsDraw(false);
			trunState = TrunState::End;
		}
	}
}

void CardManager::EndTrun(TrunState& trunState) {
	isHoldCard = false;
	if (!zoneMap[CardZone::Execution].empty()) {
		return;
	}
	std::vector<Card*> handCards = zoneMap[CardZone::Hand];
	std::vector<std::unique_ptr<CardMove>> moves;
	for (auto& card : handCards) {
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(card, CemeteryCardPos(), 0.5f, false);
		moves.push_back(std::move(move));
		MoveCard(card, CardZone::Cemetery);
	}
	if (!moves.empty()) {
		AddCardMove(std::move(moves));
	}
	if (!cardMoves.empty()) {
		return;
	}

	trunState = TrunState::Start;

}

void CardManager::PlayerInput() {
	Input* input = Input::GetInstance();
	Vector2 mousePos = input->GetMousePos();
	if (!isHoldCard) {
		if (input->TriggerMouseButton(0)) {
			int index = 0;
			for (const auto& card : zoneMap[CardZone::Hand]) {
				if (card->IsDraw() && card->IsOnCollision(mousePos)) {
					card->SetIsMove(false);
					isHoldCard = true;
					holdCardIndex = index;
					break;
				}
				index++;
			}
		}
	} else {

		if (input->PressMouseButton(0)) {
			zoneMap[CardZone::Hand][holdCardIndex]->SetPos(mousePos);
			zoneMap[CardZone::Hand][holdCardIndex]->SetIsMove(false);
		} else if (input->ReleaseMouseButton(0)) {
			if (cardExecutionField->IsOnCollision(mousePos)) {
				zoneMap[CardZone::Hand][holdCardIndex]->SetIsDraw(false);
				MoveCard(zoneMap[CardZone::Hand][holdCardIndex], CardZone::Execution);
			}
			isHoldCard = false;
			HandAdjustment();
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
	handMove->Initialize(this, handCards[0], handCards, 0.3f);
	moves.push_back(std::move(handMove));
	AddCardMove(std::move(moves));

	moves.clear();
	// 墓地に送るカードを移動
	for (const auto& card : removeCards) {
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(card, CemeteryCardPos(), 0.5f, false);
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
		pos = HandCardPos(i);
		card->SetNewPos(pos);
		card->SetIsDraw(true);
		i++;
	}
}

void CardManager::ReShuffleDeck() {
	std::vector<Card*> cemeteryCards = zoneMap[CardZone::Cemetery];
	Vector2 pos = Vector2{-100.0, -100.0f};
	for (const auto& card : cemeteryCards) {
		card->SetPos(pos);
		card->RessetVariable();
		card->SetIsDraw(false);
		MoveCard(card, CardZone::Deck);
	}
	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), g);

	std::unique_ptr<CardShuffleMove> shuffleMove = std::make_unique<CardShuffleMove>();
	shuffleMove->Initialize(this, cemeteryCards, shuffleSE, 1.0f);
	std::vector<std::unique_ptr<CardMove>> moves;
	moves.push_back(std::move(shuffleMove));
	AddCardMove(std::move(moves));

}

void CardManager::ExecutionCard() {
	if (!zoneMap[CardZone::Execution].empty()) {
		if (zoneMap[CardZone::Execution].front()->Effect()) {
			MoveCard(zoneMap[CardZone::Execution].front(), CardZone::Cemetery);
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
		MoveCard(zoneMap[CardZone::Deck].front(), CardZone::Open);
	}
	if (!isCommand) {
		int i = 0;
		int size = static_cast<int>(zoneMap[CardZone::Open].size()) - 1;
		for (const auto& card : zoneMap[CardZone::Open]) {
			pos.x = 640.0f - (size / 2.0f - i) * 122.0f;
			pos.y = 240.0f;
			card->SetNewPos(pos);
			i++;
		}
	}
	return result;
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

Vector2 CardManager::GetCardPos(CardZone zone, int index) {
	if (zone == CardZone::Hand) {
		return HandCardPos(index);
	} else if (zone == CardZone::Open) {
		return OpenCardPos(index);
	} else if (zone == CardZone::Cemetery) {
		return CemeteryCardPos();
	}else if(zone == CardZone::Deck){
		return Vector2(640.0f, -160.0f);
	}

	return Vector2();
}

Vector2 CardManager::HandCardPos(int index) {
	Vector2 pos;
	int size = static_cast<int>(zoneMap[CardZone::Hand].size()) - 1;
	pos.x = 640.0f - (size / 2.0f - index) * 122.0f;
	pos.y = 720.0f - 80.0f;
	return pos;
}

Vector2 CardManager::OpenCardPos(int index) {
	Vector2 pos;
	int size = static_cast<int>(zoneMap[CardZone::Open].size()) - 1;
	pos.x = 640.0f - (size / 2.0f - index) * 126.0f;
	pos.y = 240.0f;
	return pos;
}

Vector2 CardManager::CemeteryCardPos() {
	return Vector2(-150.0f, 80.0f);
}
