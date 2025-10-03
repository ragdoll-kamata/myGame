#include "CardManager.h"
#include <iostream>
#include <filesystem>
#include <vector>

#include <fstream>
#include <sstream>

#include <Input.h>

namespace fs = std::filesystem;

void CardManager::Initialize() {
	std::random_device rd; // 乱数の種
	g.seed(rd());
	endTurnButton = std::make_unique<Button>();
	endTurnButton->Initialize({ 1100.0f, 600.0f }, { 200.0f, 100.0f }, "white.png", { 1.0f, 0.0f, 0.0f, 1.0f });
	endTurnButton->SetIsDraw(false);
	startOpenButton = std::make_unique<Button>();
	startOpenButton->Initialize({ 200.0f, 600.0f }, { 200.0f, 100.0f }, "white.png", { 0.0f, 1.0f, 0.0f, 1.0f });
	startOpenButton->SetIsDraw(false);
	startOpenEndButton = std::make_unique<Button>();
	startOpenEndButton->Initialize({ 440.0f, 600.0f }, { 200.0f, 100.0f }, "white.png", { 0.0f, 0.0f, 1.0f, 1.0f });
	startOpenEndButton->SetIsDraw(false);
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
		if (loadCardMap.contains(line)) {
			for (int i = 0; i < 10; i++) {
				std::unique_ptr<Card> card(new Card());
				card->InitializeCard(loadCardMap[line].get());
				card->SetCardManager(this);
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
	StartTrun(trunState);
	for (const auto& card : allCards) {
		card->Update();
	}
	startOpenButton->Update();
	startOpenEndButton->Update();
}

void CardManager::Draw() {
	for (const auto& card : allCards) {
		card->Draw();
	}
	//endTurnButton->Draw();
	startOpenButton->Draw();
	startOpenEndButton->Draw();
}

void CardManager::TextDraw() {
	for (const auto& card : allCards) {
		card->TextDraw();
	}

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
				nowOpenCard = 0;
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
			int lIndex = 0;
			int dIndex = 0;
			for (const auto& card : zoneMap[CardZone::Open]) {
				if (card->GetElement() == CardElement::Light) {
					lIndex++;
				}
				if (card->GetElement() == CardElement::Darkness) {
					dIndex++;
				}
			}
			int index = 0;
			if (lIndex > dIndex) {
				index = 1;
			} else if(lIndex < dIndex){
				index = 2;
			}
			for (const auto& card : zoneMap[CardZone::Open]) {
				if (index == 0) {
					MoveCard(card, CardZone::Hand);
				} else {
					if (card->GetElement() == CardElement::Light && index == 1) {
						MoveCard(card, CardZone::Hand);
					}
					if (card->GetElement() == CardElement::Darkness && index == 2) {
						MoveCard(card, CardZone::Hand);
					}
					if (card->GetElement() == CardElement::None) {
						MoveCard(card, CardZone::Hand);
					}
				}
			}
			for (const auto& card : zoneMap[CardZone::Open]) {
				MoveCard(card, CardZone::Cemetery);
				card->SetIsDraw(false);
			}
			trunState = TrunState::Main;
		}
		//

	}


}

void CardManager::MainTrun(TrunState& trunState) {
}

void CardManager::EndTrun(TrunState& trunState) {
}

/////////////////////////////////////////////////////////////////////////////

std::vector<Card*> CardManager::OpenDeck(int num) {
	std::vector<Card*> result;
	Vector2 pos{640.0f, -160.0f};
	for (int i = 0; i < num; i++) {
		zoneMap[CardZone::Deck].front()->SetPos(pos);
		result.push_back(zoneMap[CardZone::Deck].front());
		MoveCard(zoneMap[CardZone::Deck].front(), CardZone::Open);
	}

	for (const auto& card : result) {
		card->SetIsDraw(true);
	}
	int i = 0;
	int size = static_cast<int>(zoneMap[CardZone::Open].size()) - 1;
	for (const auto& card : zoneMap[CardZone::Open]) {
		pos.x = 640.0f - (size / 2.0f - i) * 122.0f;
		pos.y = 240.0f;
		card->SetNewPos(pos);
		i++;
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
			std::unique_ptr<LoadCard> loadCard = std::make_unique<LoadCard>();
			loadCard->LoadCardFile(path.string());
			loadCardMap.insert(std::pair(fileName, std::move(loadCard)));
		}
	}


}