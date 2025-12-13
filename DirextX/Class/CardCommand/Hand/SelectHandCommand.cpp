#include "SelectHandCommand.h"
#include "Input.h"
#include "CardManager.h"
#include "ErrorMessage.h"

bool SelectHandCommand::Initialize(const std::string& minSelect, const std::string& maxSelect, const std::string& selectCard, const std::string& notSelectCard) {
	if (selectCard.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	if (notSelectCard.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	if (minSelect.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"中身ないよ");
		return false; // Error: minSelect is empty
	}
	if (minSelect.front() != '#') {
		std::unique_ptr<Card> cardptr = std::make_unique<Card>();
		int number = ParseInt(minSelect, cardptr.get());
		if (number < 0) {
			ErrorMessage::GetInstance()->SetMessage(U"数字に変換できないよ");
			return false;
		}
	}
	if (maxSelect.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"中身ないよ");
		return false; // Error: maxSelect is empty
	}
	if (maxSelect.front() != '#') {
		std::unique_ptr<Card> cardptr = std::make_unique<Card>();
		int number = ParseInt(maxSelect, cardptr.get());
		if (number < 0) {
			ErrorMessage::GetInstance()->SetMessage(U"数字に変換できないよ");
			return false;
		}
	}
	selectCard_ = selectCard;
	notSelectCard_ = notSelectCard;
	minSelect_ = minSelect;
	maxSelect_ = maxSelect;

	return true; // Initialization successful
}

ExecuteResult SelectHandCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Error: card is null
	}
	// 開始処理
	if (!isStart_) {
		isStart_ = true;
		selectCards_ = cardManager_->GetZoneCards(CardZone::Hand);
		for (Card* c : selectCards_) {
			cardManager_->AddSelectCard(c);
		}

		minSelectNum_ = ParseInt(minSelect_, card);
		maxSelectNum_ = ParseInt(maxSelect_, card);
		if (minSelectNum_ == 0) {
			cardManager_->SetEndSelectButtonColorV(1.0f);
		} else {
			cardManager_->SetEndSelectButtonColorV(0.3f);
		}
		cardManager_->SetEndSelectButtonHandVector();
		return ExecuteResult::Standby;
	}

	// 選択処理
	cardManager_->SetIsSelectCard(true);
	Vector2 mousePos = Input::GetInstance()->GetMousePos();
	if (Input::GetInstance()->TriggerMouseButton(0)) {
		for (Card* c : selectCards_) {
			if (c->IsOnCollision(mousePos)) {
				if (!c->IsWaku()) {
					if (nawSelectCount_ < maxSelectNum_) {
						c->SetWaku(true);
						nawSelectCount_++;
					}
				} else {
					c->SetWaku(false);
					nawSelectCount_--;
				}
			}
		}
	}
	if (nawSelectCount_ < minSelectNum_) {
		cardManager_->SetEndSelectButtonColorV(0.3f);
	} else {
		cardManager_->SetEndSelectButtonColorV(1.0f);
		if (Input::GetInstance()->TriggerMouseButton(0)) {
			if (cardManager_->IsEndSelectButton()) {
				for (Card* c : selectCards_) {
					if (c->IsWaku()) {
						card->AddCard(selectCard_, c);
					} else {
						card->AddCard(notSelectCard_, c);
					}
					c->SetWaku(false);
				}
				cardManager_->SetIsSelectCard(false);
				isStart_ = false;
				nawSelectCount_ = 0;
				return ExecuteResult::Normal;
			}
		}
	}



	return ExecuteResult::Standby;
}