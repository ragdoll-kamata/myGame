#pragma once
#include "CardMove.h"
#include <vector> 
class CardShuffleMove : public CardMove {
public:
	void Initialize(std::vector<Card*> shuffleCards, int SH, float time);
	void Update() override;
private:
	std::vector<Card*> shuffleCards_;
	bool isStart_ = false;
	int SH_;
};

