#include "CardManager.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "LoadCard.h"
namespace fs = std::filesystem;

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
				zoneMap[CardZone::Deck].push_back(card.get());
				allCards.push_back(std::move(card));
			}
		}
	}
	
	std::shuffle(allCards.begin(), allCards.end(), g);
	return true;
}

std::vector<Card*> CardManager::OpenDeck(int num) {
	std::vector<Card*> result;
	for (int i = 0; i < num; i++) {
		result.push_back(zoneMap[CardZone::Deck].front());
		MoveCard(zoneMap[CardZone::Deck].front(), CardZone::Open);
	}

	for (const auto& card : result) {
		card->SetIsDraw(true);
	}

	return result;
}

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
	std::random_device rd; // 乱数の種
	g.seed(rd());

}

void CardManager::MoveCard(Card* card, CardZone cardZone) {
	
	std::vector<Card*>& zone = zoneMap[card->GetZone()];
	zone.erase(std::remove(zone.begin(), zone.end(), card), zone.end());
	
	card->SetZone(cardZone);
	zoneMap[cardZone].push_back(card);
}
