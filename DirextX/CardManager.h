#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Card.h"
struct CardFile {
	std::string modName;
	std::string filePath;
	std::string fileName;
};
class CardManager {
public:


	std::vector<Card*> OpenDeck(int num);

	void AllCardLoad(const std::string& file);


	void MoveCard(Card* card, CardZone cardZone);
private:
	//　カード場
	std::vector<std::unique_ptr<Card>> allCards;
	std::unordered_map<CardZone, std::vector<Card*>> zoneMap{
		{CardZone::Deck, {}},
		{CardZone::Hand, {}},
		{CardZone::Field, {}},
		{CardZone::Cemetery, {}},
		{CardZone::Execution, {}},
		{CardZone::Open, {}}
	};

	std::unordered_map<std::string,std::unique_ptr<LoadCard>> loadCardMap;
};

