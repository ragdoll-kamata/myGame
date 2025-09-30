#include "CardMove.h"
#include "DeltaTime.h"
#include "MathUtility.h"

#include "Card.h"

void CardMove::Initialize(float time) {
	time_ = time;
}

bool CardMove::Updata() {
	nawTime += DeltaTime::GetInstance()->GetDeltaTime();
	nawTime = std::min<float>(nawTime, time_);
	float t = nawTime / time_;
	for (int i = 0; i < cards.size(); i++) {
		Vector2 pos;
		Card* card = cards[i];
		Vector2 start = start_[std::min<int>(i, static_cast<int>(start_.size() - 1))];
		Vector2 end = end_[std::min<int>(i, static_cast<int>(end_.size() - 1))];
		pos.x = MathUtility::Lerp(start.x, end.x, t);
		pos.y = MathUtility::Lerp(start.y, end.y, t);
		card->SetPos(pos);
	}
	if (t == 1.0f) {
		return true;
	}
	return false;
}
