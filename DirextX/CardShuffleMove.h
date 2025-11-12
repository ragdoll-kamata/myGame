#pragma once
#include "CardMove.h"
#include <vector> 
class CardManager;
class CardShuffleMove : public CardMove {
public:
	void Initialize(CardManager* cardManager, std::vector<Card*> shuffleCards, int SH, float time);
	void Update() override;
private:
	CardManager* cardManager_;
	std::vector<Card*> shuffleCards_;
	bool isStart_ = false;
	int SH_;
};

