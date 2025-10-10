#pragma once
#include "Vector2.h"
class Card;
class MoveCardCo {
public:
	void Initialize(Card* card, Vector2 pos, float time);
	void Update();
	bool IsEnd() {
		return isEnd_;
	}

private:
	Vector2 pos_;
	Vector2 startPos_;
	Card* card_;
	float time_ = 0.0f;
	float nowTime_ = 0.0f;
	bool isEnd_ = false;
};

