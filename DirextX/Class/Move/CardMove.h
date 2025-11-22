#pragma once
#include "Vector2.h"
class Card;
class CardMove {
public:
	~CardMove();
	void Initialize(Card* card, Vector2 pos, float time, float isEndDraw);
	virtual void Update();

	void SetStart();

	void End();

	bool IsEnd() {
		return isEnd_;
	}

protected:
	Vector2 pos_;
	Vector2 startPos_;
	Card* card_;
	float time_ = 0.0f;
	float nowTime_ = 0.0f;
	bool isEnd_ = false;
	bool isEndDraw_ = false;

};
