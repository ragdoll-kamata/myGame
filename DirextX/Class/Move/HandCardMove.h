#pragma once
#include "CardMove.h"
#include <vector>
class CardManager;
class HandCardMove : public CardMove {
public:
	void Initialize(CardManager* cardManager, std::vector<Card*> cards, float time);
	void Update() override;
private:
	CardManager* cardManager_;
	std::vector<Card*> cards_;
	bool isStart_ = false;
};

