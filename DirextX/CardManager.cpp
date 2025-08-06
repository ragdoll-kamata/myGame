#include "CardManager.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include "LoadCard.h"
namespace fs = std::filesystem;

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
			std::string modName = path.parent_path().filename().string(); // mod名
			std::string fileName = path.filename().string();
			cardFiles.push_back({modName, path.string(), fileName});
			std::unique_ptr<LoadCard> loadCard = std::make_unique<LoadCard>();
			loadCard->LoadCardFile(path.string());
			loadCardMap.insert(std::pair(fileName, std::move(loadCard)));
		}
	}
}

void CardManager::MoveCard(Card* card, CardZone cardZone) {
	
	std::vector<Card*>& zone = zoneMap[card->GetZone()];
	zone.erase(std::remove(zone.begin(), zone.end(), card), zone.end());
	
	card->SetZone(cardZone);
	zoneMap[cardZone].push_back(card);
}
