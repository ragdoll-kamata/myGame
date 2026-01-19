#include "PlayerInput.h"

#include "CardManager.h"
#include "ShopManager.h"
#include "UIManager.h"


void PlayerInput::Initialize(CardManager* cardManager, ShopManager* shopManager, UIManager* uiManager, ResourceManager* resourceManager) {
	cardManager_ = cardManager;
	shopManager_ = shopManager;
	uiManager_ = uiManager;
	resourceManager_ = resourceManager;
	input_ = Input::GetInstance();
}

void PlayerInput::Update(TurnState& turnState) {
	monsePos_ = input_->GetMousePos();
	if (cardManager_->IsSelectCard()) {
		SelectUpdate();
		return;
	}
	if(turnMap.find(turnState) == turnMap.end()) {
		return;
	}
	turnMap[turnState](turnState);
}

void PlayerInput::MainTurnUpdate(TurnState& turnState) {
	
	if(!cardManager_->IsHoldCard()) {
		if (input_->PressMouseButton(0)) {
			int index = cardManager_->HandCardCollision(monsePos_);
			if (index != -1) {
				cardManager_->SetHoldCard(index);
			}

			
		}
		return;
	}

	if(input_->PressMouseButton(0)) {
		Card* card = cardManager_->GetZoneCards(CardZone::Hand)[cardManager_->GetHoldCardIndex()];
		card->SetPos(monsePos_);
		card->SetIsMove(false);
		return;
	}


	if (input_->ReleaseMouseButton(0)) {
		int holdCardIndex = cardManager_->GetHoldCardIndex();

		if (TryExecution(monsePos_, holdCardIndex)) {
		}else if (TryBuilding(monsePos_, holdCardIndex)) {
		}
		cardManager_->SetIsHoldCard(false);
		cardManager_->HandAdjustment();
	}
}

void PlayerInput::ShopTurnUpdate(TurnState& turnState) {
}

void PlayerInput::SelectUpdate() {
	std::vector<Card*> selectCards = cardManager_->GetSelectCards();
	int minSelectCard = 0;
	int maxSelectCard = 0;
	cardManager_->GetMinMaxSelectCard(minSelectCard, maxSelectCard);

	if (Input::GetInstance()->TriggerMouseButton(0)) {
		for (Card* c : selectCards) {
			if (c->IsOnCollision(monsePos_)) {
				if (!c->IsWaku()) {
					if (nawSelectCount_ < maxSelectCard) {
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
	if (nawSelectCount_ < minSelectCard) {
		uiManager_->SetEndSelectButtonColorV(0.3f);
	} else {
		uiManager_->SetEndSelectButtonColorV(1.0f);
		if (Input::GetInstance()->TriggerMouseButton(0)) {
			if (uiManager_->IsOnCollisionEndSelectButton(monsePos_)) {
				nawSelectCount_ = 0;
				cardManager_->SetIsEndSelect(true);
			}
		}
	}
}

bool PlayerInput::TryExecution(Vector2 mousePos, int holdCardIndex) {
	if (uiManager_->IsOnCollisionExecutionField(mousePos)) {
		Card* card = cardManager_->GetZoneCards(CardZone::Hand)[holdCardIndex];
		if (card->GetType() == CardType::Ritual) {
			if (card->IsCostSufficient(resourceManager_)) {
				card->SetIsDraw(false);
				cardManager_->SetEffectStandby(card);
				cardManager_->MoveCard(card, CardZone::Execution);
				return true;
			}
		}
	}
	return false;
}

bool PlayerInput::TryBuilding(Vector2 mousePos, int holdCardIndex) {
	int i = uiManager_->IsOnCollisionFieldCardField(mousePos);
	if(i!= -1) {
		Card* card = cardManager_->GetZoneCards(CardZone::Hand)[holdCardIndex];
		if (card->GetType() == CardType::Building) {
			if (card->IsCostSufficient(resourceManager_)) {
				if (!uiManager_->GetFieldCardFieldIsOn(i)) {
					uiManager_->SetFieldCardFieldIsOn(i, true);
					cardManager_->SetFieldCardIndex(card, i);
					card->SetIsDraw(true);

					std::unique_ptr<CardMove> moveCard = std::make_unique<CardMove>();
					moveCard->Initialize(card, uiManager_->GetCardPos(CardZone::Field, i, 0), 0.5f, true);
					std::vector<std::unique_ptr<CardMove>> moves;
					moves.push_back(std::move(moveCard));
					cardManager_->AddCardMove(std::move(moves));

					card->InitializeDurability();
					cardManager_->MoveCard(card, CardZone::Field);
					return true;
				}
			}
		}
	}
	return false;
}
