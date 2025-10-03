#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <random>
#include <functional>
#include "Card.h"
#include "LoadCard.h"
#include "Button.h"
#include "TrunState.h"
struct CardFile {
	std::string modName;
	std::string filePath;
	std::string fileName;
};

class CardManager {
public:
	void Initialize();

	bool StartCardSet();

	void Update(TrunState& trunState);

	void Draw();

	void TextDraw();

private:
	void StartTrun(TrunState& trunState);

	void MainTrun(TrunState& trunState);

	void EndTrun(TrunState& trunState);

	void OpenDeckAdjustment();

	void HandAdjustment();
public:


	std::vector<Card*> OpenDeck(int num);




	void MoveCard(Card* card, CardZone cardZone);
public:
	void AllCardLoad(const std::string& file);
private:
	std::mt19937 g;
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

	std::unordered_map<std::string, std::unique_ptr<LoadCard>> loadCardMap;
private:
	std::unique_ptr<Button> endTurnButton = nullptr;

	std::unique_ptr<Button> startOpenButton = nullptr;
	std::unique_ptr<Button> startOpenEndButton = nullptr;
	std::unordered_map<TrunState, std::function<void(TrunState&)>> trunMap{
		{TrunState::Start, [&](TrunState i) {return StartTrun(i); }},
		{TrunState::Main,  [&](TrunState i) {return MainTrun(i);  }},
		{TrunState::End,   [&](TrunState i) {return EndTrun(i);   }},
	};
	bool isStartOpen = true;
	bool isEndStartTrun = false;

	const int startMaxOpenCard = 5;
	int nowOpenCard = 0;
};

