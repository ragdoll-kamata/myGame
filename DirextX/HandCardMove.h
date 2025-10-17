#pragma once
#include "CardMove.h"
#include <vector>
class HandCardMove : public CardMove {
public:
	void Initialize(Card* card, std::vector<Card*> cards, float time);
	void Update() override;
private:
	std::vector<Card*> cards_;
	bool isStart_ = false;
};

