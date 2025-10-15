#pragma once
#include "CardMove.h"
class HandCardMove : public CardMove {
public:
	void Initialize(Card* card, float time);
	void Update() override;
private:
	bool isStart_ = false;
};

