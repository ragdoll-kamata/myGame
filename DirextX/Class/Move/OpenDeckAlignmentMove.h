#pragma once
#include "CardMove.h"
class CardManager;
class OpenDeckAlignmentMove : public CardMove {
public:
	void Initialize(CardManager* cardManager);
	void Update() override;
private:
	CardManager* cardManager_ = nullptr;
};

