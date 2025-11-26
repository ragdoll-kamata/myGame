#include "SelectOpenDeckCommand.h"
#include "CardManager.h"
#include "ErrorMessage.h"
#include "Input.h"

bool SelectOpenDeckCommand::Initialize(std::string num, std::string minSelect, std::string maxSelect, std::string selectCard, std::string notSelectCard) {
	if (selectCard.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	if (notSelectCard.front() != '$') {
		ErrorMessage::GetInstance()->SetMessage(U"カード変数になってないよ");
		return false; // Error: card does not start with '$'
	}
	if (num.empty()) {
		ErrorMessage::GetInstance()->SetMessage(U"中身ないよ");
		return false; // Error: num is empty
	}
	if (num.front() != '#') {
		std::unique_ptr<Card> cardptr = std::make_unique<Card>();
		int number = ParseInt(num, cardptr.get());
		if (number < 0) {
			ErrorMessage::GetInstance()->SetMessage(U"数字に変換できないよ");
			return false;
		}
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

	num_ = num;
	selectCard_ = selectCard;
	notSelectCard_ = notSelectCard;
	minSelect_ = minSelect;
	maxSelect_ = maxSelect;
	return true; // Initialization successful
}

ExecuteResult SelectOpenDeckCommand::Execute(Card* card) {
	if (card == nullptr) {
		ErrorMessage::GetInstance()->SetMessage(U"カードがないよ");
		return ExecuteResult::Error; // Error: card is null
	}
	// 開始処理
	if (!isStart_) {
		isStart_ = true;
		int number = ParseInt(num_, card);
		selectCards_ = cardManager_->OpenDeck(number, true);
		int i = 0;
		std::vector<Card*> openCards = cardManager_->GetZoneCards(CardZone::Open);
		std::vector<std::unique_ptr<CardMove>> moves;
		for (Card* c : selectCards_) {
			cardManager_->AddSelectCard(c);
			c->SetIsDraw(true);
		}
		for (Card* c : openCards) {
			Vector2 pos = cardManager_->GetCardPos(CardZone::Open, i);
			std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
			move->Initialize(c, pos, 0.5f, true);
			moves.push_back(std::move(move));
			i++;
		}
		cardManager_->AddCardMove(std::move(moves));
		if (minSelectNum_ == 0) {
			cardManager_->SetEndSelectButtonColorV(1.0f);
		} else {
			cardManager_->SetEndSelectButtonColorV(0.3f);
		}
		minSelectNum_ = ParseInt(minSelect_, card);
		maxSelectNum_ = ParseInt(maxSelect_, card);
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