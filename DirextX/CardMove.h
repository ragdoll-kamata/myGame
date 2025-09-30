#pragma once
#include <vector>
#include "Vector2.h"
class Card;
class CardMove {
public:
	void Initialize( float time);

	bool Updata();

	void AddCard(Card* card) {
		cards.push_back(card);
	}
	void AddStartPos(Vector2 start) {
		start_.push_back(start);
	}
	void AddEndPos(Vector2 end) {
		end_.push_back(end);
	}
private:
	std::vector<Card*>cards;
	std::vector<Vector2> start_;
	std::vector<Vector2> end_;
	float time_;
	float nawTime;
};

