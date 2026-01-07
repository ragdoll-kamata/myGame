#include "CardManager.h"
#include <iostream>
#include <filesystem>
#include <vector>

#include <fstream>
#include <sstream>

#include <Input.h>

#include "TextCommon.h"
#include "SpriteCommon.h"

#include "HandCardMove.h"
#include "CardShuffleMove.h"
#include "OpenDeckAlignmentMove.h"

#include "Audio.h"

namespace fs = std::filesystem;

void CardManager::Initialize() {
	std::random_device rd; // 乱数の種
	seed.seed(rd());
	endTurnButton = std::make_unique<Button>();
	endTurnButton->Initialize({950.0f, 500.0f}, {150.0f, 50.0f}, "white.png", {1.0f, 0.0f, 0.0f, 1.0f});
	endTurnButton->SetIsDraw(false);

	startOpenButton = std::make_unique<Button>();
	startOpenButton->Initialize({200.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 1.0f, 0.0f, 1.0f});
	startOpenButton->SetIsDraw(false);

	startOpenEndButton = std::make_unique<Button>();
	startOpenEndButton->Initialize({440.0f, 600.0f}, {200.0f, 100.0f}, "white.png", {0.0f, 0.0f, 1.0f, 1.0f});
	startOpenEndButton->SetIsDraw(false);

	cardExecutionField = std::make_unique<Button>();
	cardExecutionField->Initialize({1150.0f, 400.0f}, {120.0f * 1.2f, 160.0f * 1.2f}, "white.png", {0.0f, 1.0f, 1.0f, 1.0f});
	cardExecutionField->SetIsDraw(true);
	shuffleSE = Audio::GetInstance()->LoadSound("cardShuffle.mp3");

	costBackSprite = std::make_unique<Sprite>();
	costBackSprite->Initialize("white.png");
	costBackSprite->SetSize({60.0f, 40.0f});
	costBackSprite->SetPosition({0.0f, 0.0f});
	costBackSprite->SetColor({0.9f, 0.9f, 0.5f, 1.0f});

	costBackSprite2 = std::make_unique<Sprite>();
	costBackSprite2->Initialize("white.png");
	costBackSprite2->SetSize({60.0f, 40.0f});
	costBackSprite2->SetPosition({60.0f, 0.0f});
	costBackSprite2->SetColor({0.5f, 0.0f, 0.5f, 1.0f});

	lightCostText = std::make_unique<Text>();
	lightCostText->Initialize(U"00", {10.0f, 0.0f}, 20000.0f);
	lightCostText->Update();
	lightCostText->CalcFitSize(40.0f);

	darknessCostText = std::make_unique<Text>();
	darknessCostText->Initialize(U"00", {70.0f, 0.0f}, 2000.0f);
	darknessCostText->Update();
	darknessCostText->CalcFitSize(40.0f);

	selectCardBackSprite = std::make_unique<Sprite>();
	selectCardBackSprite->Initialize("white.png");
	selectCardBackSprite->SetSize({1280.0f, 720.0f});
	selectCardBackSprite->SetColor({0.0f, 0.0f, 0.0f, 0.0f});

	runawayGaugeBackSprite = std::make_unique<Sprite>();
	runawayGaugeBackSprite->Initialize("white.png");
	runawayGaugeBackSprite->SetSize({30.0f, 500.0f});
	runawayGaugeBackSprite->SetPosition({1250.0f, 100.0f});
	runawayGaugeBackSprite->SetAnchorPoint({0.5f, 0.0f});
	runawayGaugeBackSprite->SetColor({0.2f, 0.2f, 0.2f, 1.0f});

	runawayGaugeSprite = std::make_unique<Sprite>();
	runawayGaugeSprite->Initialize("white.png");
	runawayGaugeSprite->SetSize({30.0f, 0.0f});
	runawayGaugeSprite->SetPosition({1250.0f, 600.0f});
	runawayGaugeSprite->SetAnchorPoint({0.5f, 1.0f});
	runawayGaugeSprite->SetColor({1.0f, 0.0f, 1.0f, 1.0f});

	endSelectButton = std::make_unique<Button>();
	endSelectButton->Initialize({640.0f, 650.0f}, {200.0f, 100.0f}, "white.png", {1.0f, 0.0f, 1.0f, 1.0f});
	endSelectButton->SetIsDraw(false);

	for (int i = 0; i < maxFieldCard; i++) {
		FieldCard fi;
		fi.field = std::make_unique<Button>();
		fi.field->Initialize(FieldCardPos(i), {120.0f * 1.1f, 160.0f * 1.1f}, "white.png", {0.4f, 0.9f, 0.4f, 0.9f});
		fi.field->SetIsDraw(true);
		fieldCardField.push_back(std::move(fi));
	}

}

bool CardManager::StartCardSet() {
	std::ifstream file("StartCard.txt");
	if (!file) {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return false;
	}

	std::string line;
	int lineNumber = 0;
	while (std::getline(file, line)) {
		std::string name;
		std::string num;
		bool is = false;
		for (char c : line) {
			if (c == ' ') {
				continue;
			}
			if (c == ',') {
				is = true;
				continue;
			}
			if (!is) {
				name += c;
			} else {
				num += c;
			}

		}

		if (CardDataMap.contains(name)) {
			int number = std::stoi(num);
			for (int i = 0; i < number; i++) {
				std::unique_ptr<Card> card(new Card());
				card->InitializeCard(CardDataMap[name].get());
				zoneMap[CardZone::Deck].push_back(card.get());
				card->SetZone(CardZone::Deck);
				allCards.push_back(std::move(card));
			}
		}
	}

	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), seed);
	return true;
}

void CardManager::Update(TrunState& trunState) {
	trunMap[trunState](trunState);

	if (!isMove) {
		ExecutionCard();
	}

	if (effectStandby_.empty()) {
		for (auto& fi : fieldCardField) {
			if (fi.card) {
				if (fi.card->GetDurability() <= 0) {
					std::unique_ptr<CardMove> moveCard = std::make_unique<CardMove>();
					moveCard->Initialize(fi.card, CemeteryCardPos(), 0.5f, false);
					std::vector<std::unique_ptr<CardMove>> moves;
					moves.push_back(std::move(moveCard));
					AddCardMove(std::move(moves));

					MoveCard(fi.card, CardZone::Cemetery);

					fi.card = nullptr;
					fi.isOn = false;
				}
			}
		}
	}

	CardMoveUpdate();

	for (const auto& card : allCards) {
		card->Update();
	}

	endTurnButton->Update();
	startOpenButton->Update();
	startOpenEndButton->Update();

	cardExecutionField->Update();
	for (const auto& fi : fieldCardField) {
		fi.field->Update();
	}

	lightCostText->Update();
	darknessCostText->Update();
	costBackSprite->Updata();
	costBackSprite2->Updata();

	float gaugeH = (runawayGauge_ / maxRunawayGauge_) * 500.0f;
	runawayGaugeSprite->SetSize({30.0f, gaugeH});
	runawayGaugeSprite->Updata();
	runawayGaugeBackSprite->Updata();

	if (isSelectCard) {
		selectCardBackSpriteAlpha += 0.05f;
		if (selectCardBackSpriteAlpha > selectCardBackSpriteMaxAlpha) {
			selectCardBackSpriteAlpha = selectCardBackSpriteMaxAlpha;
		}
		selectCardBackSprite->SetColor({0.0f, 0.0f, 0.0f, selectCardBackSpriteAlpha});
	} else {
		selectCardBackSpriteAlpha -= 0.05f;
		if (selectCardBackSpriteAlpha < 0.0f) {
			selectCardBackSpriteAlpha = 0.0f;
		}
		selectCardBackSprite->SetColor({0.0f, 0.0f, 0.0f, selectCardBackSpriteAlpha});

	}
	selectCardBackSprite->Updata();
	endSelectButton->Update();
}

void CardManager::Draw() {

	SpriteCommon::GetInstance()->PreDraw();
	endTurnButton->Draw();
	runawayGaugeBackSprite->Draw();
	runawayGaugeSprite->Draw();


	cardExecutionField->Draw();
	for (const auto& fi : fieldCardField) {
		fi.field->Draw();
	}

	costBackSprite->Draw();
	costBackSprite2->Draw();
	TextCommon::GetInstance()->PreDraw();

	lightCostText->Draw();
	darknessCostText->Draw();
	if (effectTextCard_) {
		effectTextCard_->EffectTextDraw();
	}

	
	CardDraw();

	SpriteCommon::GetInstance()->PreDraw();

	startOpenButton->Draw();
	startOpenEndButton->Draw();

	if (isSelectCard) {

		selectCardBackSprite->Draw();
		endSelectButton->Draw();

		for (const auto& card : selectCards) {
			SpriteCommon::GetInstance()->PreDraw();
			card->Draw();

			TextCommon::GetInstance()->PreDraw();
			card->TextDraw();
			TextCommon::GetInstance()->PostDraw();
		}
	}
}

void CardManager::TextDraw() {
	for (const auto& card : allCards) {
		card->TextDraw();
	}

}

void CardManager::AddCardMove(std::vector<std::unique_ptr<CardMove>> moveCard) {
	cardMoves.push_back(std::move(moveCard));
}

void CardManager::CardDraw() {
	TextCommon::GetInstance()->PostDraw();
	for (const auto& card : allCards) {
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}

	for (auto* card : zoneMap[CardZone::Hand]) {
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}
	if (isHoldCard) {
		Card* card = zoneMap[CardZone::Hand][holdCardIndex];
		SpriteCommon::GetInstance()->PreDraw();
		card->Draw();

		TextCommon::GetInstance()->PreDraw();
		card->TextDraw();
		TextCommon::GetInstance()->PostDraw();
	}
}

void CardManager::StartTrun(TrunState& trunState) {
	Input* input = Input::GetInstance();
	startOpenButton->SetIsDraw(true);
	startOpenEndButton->SetIsDraw(true);
	if (!isEndStartTrun) {
		if (isStartOpen) {
			OpenDeck(1);
			isStartOpen = false;
			nowOpenCard++;
			if (nowOpenCard >= startMaxOpenCard) {
				isEndStartTrun = true;
			}
		}
		Vector2 mousePos = input->GetMousePos();
		if (input->TriggerMouseButton(0)) {
			if (startOpenButton->IsOnCollision(mousePos) && nowOpenCard < startMaxOpenCard) {
				isStartOpen = true;
			}
			if (startOpenEndButton->IsOnCollision(mousePos)) {
				isEndStartTrun = true;
			}
		}
	} else {
		startOpenButton->SetIsDraw(false);
		startOpenEndButton->SetIsDraw(false);
		bool is = false;
		for (const auto& card : zoneMap[CardZone::Open]) {
			if (card->IsMove()) {
				is = true;
				break;
			}
		}
		if (IsMoveCard()) {
			is = true;
		}
		if (!is) {
			isEndStartTrun = false;
			isStartOpen = true;
			nowOpenCard = 0;
			OpenDeckAdjustment();
			trunState = TrunState::Main;
		}
		//

	}


}

void CardManager::MainTrun(TrunState& trunState) {
	endTurnButton->SetIsDraw(true);
	Input* input = Input::GetInstance();
	Vector2 mousePos = input->GetMousePos();
	PlayerInput();
	if (input->TriggerMouseButton(0)) {
		if (endTurnButton->IsOnCollision(mousePos)) {
			FieldCardEffectCheck(BuildingActivationTiming::EndTurn);
			endTurnButton->SetIsDraw(false);
			runawayGauge_ += 10.0f;
			trunState = TrunState::End;
		}
	}
}

void CardManager::EndTrun(TrunState& trunState) {
	isHoldCard = false;
	if (!effectStandby_.empty()) {
		return;
	}
	if (!isEndStart) {
		std::vector<Card*> handCards = zoneMap[CardZone::Hand];
		std::vector<std::unique_ptr<CardMove>> moves;
		for (auto& card : handCards) {
			if (card->GetIsCardCharacteristics(CardCharacteristics::Protection)) {
				continue;
			}
			if (card->GetIsCardCharacteristics(CardCharacteristics::TemporaryProtection)) {
				card->SetIsCardCharacteristics(CardCharacteristics::TemporaryProtection, false);
				continue;
			}
			std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
			move->Initialize(card, CemeteryCardPos(), 0.5f, false);
			moves.push_back(std::move(move));
			MoveCard(card, CardZone::Cemetery);
		}
		if (!moves.empty()) {
			AddCardMove(std::move(moves));
		}
		isEndStart = true;
	}
	if (IsMoveCard()) {
		return;
	}
	if (!zoneMap[CardZone::Hand].empty()) {
		std::unique_ptr<HandCardMove> move = std::make_unique<HandCardMove>();
		move->Initialize(zoneMap[CardZone::Hand], 0.5f);
		std::vector<std::unique_ptr<CardMove>> moveVec;
		moveVec.push_back(std::move(move));
		AddCardMove(std::move(moveVec));
	}

	isEndStart = false;
	trunState = TrunState::Start;
}

void CardManager::PlayerInput() {
	if (isSelectCard) {
		return;
	}
	Input* input = Input::GetInstance();
	Vector2 mousePos = input->GetMousePos();

	if (!isHoldCard) {
		effectTextCard_ = nullptr;
		int index = 0;
		for (const auto& card : zoneMap[CardZone::Hand]) {
			if (card->IsDraw() && card->IsOnCollision(mousePos)) {
				effectTextCard_ = card;
				if (input->TriggerMouseButton(0)) {
					card->SetIsMove(false);
					isHoldCard = true;
					holdCardIndex = index;
					break;
				}
			}
			index++;
		}

	} else {

		if (input->PressMouseButton(0)) {
			zoneMap[CardZone::Hand][holdCardIndex]->SetPos(mousePos);
			zoneMap[CardZone::Hand][holdCardIndex]->SetIsMove(false);
		} else if (input->ReleaseMouseButton(0)) {
			if (cardExecutionField->IsOnCollision(mousePos)) {
				if (zoneMap[CardZone::Hand][holdCardIndex]->GetType() == CardType::Ritual) {
					if (zoneMap[CardZone::Hand][holdCardIndex]->IsCostSufficient(lightCost, darknessCost)) {
						CostTextUpdate();
						zoneMap[CardZone::Hand][holdCardIndex]->SetIsDraw(false);
						effectStandby_.push(zoneMap[CardZone::Hand][holdCardIndex]);
						MoveCard(zoneMap[CardZone::Hand][holdCardIndex], CardZone::Execution);
					}
				}
			} else {
				int i = 0;
				for (auto& fi : fieldCardField) {
					if (fi.field->IsOnCollision(mousePos)) {
						if (!fi.isOn) {
							if (zoneMap[CardZone::Hand][holdCardIndex]->GetType() == CardType::Building) {
								if (zoneMap[CardZone::Hand][holdCardIndex]->IsCostSufficient(lightCost, darknessCost)) {
									Vector2 pos = FieldCardPos(i);
									fi.isOn = true;
									fi.card = zoneMap[CardZone::Hand][holdCardIndex];
									zoneMap[CardZone::Hand][holdCardIndex]->SetNewPos(pos);
									zoneMap[CardZone::Hand][holdCardIndex]->InitializeDurability();
									MoveCard(zoneMap[CardZone::Hand][holdCardIndex], CardZone::Field);
								}
							}
						}
					}
					i++;
				}
			}
			isHoldCard = false;
			HandAdjustment();
		}
	}

}

void CardManager::CardMoveUpdate() {
	isMove = false;
	if (IsMoveCard()) {
		isMove = true;
		bool isEnd = true;
		for (const auto& card : cardMoves[0]) {
			card->Update();
			if (!card->IsEnd()) {
				isEnd = false;
			}
		}
		if (isEnd) {
			for (const auto& card : cardMoves[0]) {
				card->End();
			}
			cardMoves.front().clear();
			cardMoves.erase(cardMoves.begin());
			if (IsMoveCard()) {
				for (const auto& card : cardMoves[0]) {
					card->SetStart();
				}
			}
		}
	}
}

void CardManager::FieldCardEffectCheck(BuildingActivationTiming buildingActivationTiming) {
	for (const auto& card : zoneMap[CardZone::Field]) {
		if (card->GetBuildingActivationTiming() == buildingActivationTiming) {
			effectStandby_.push(card);
		}
	}
}

void CardManager::OpenDeckAdjustment() {

	int lIndex = 0;
	int dIndex = 0;
	std::vector<Card*> LightCards;
	std::vector<Card*> DarknessCards;
	std::vector<Card*> NoneCards;

	// 光と闇の数を数える
	for (const auto& card : zoneMap[CardZone::Open]) {
		if (card->GetElement() == CardElement::Light) {
			lIndex++;
			LightCards.push_back(card);
		}
		if (card->GetElement() == CardElement::Darkness) {
			dIndex++;
			DarknessCards.push_back(card);
		}
		if (card->GetElement() == CardElement::None) {
			NoneCards.push_back(card);
		}
	}
	std::list<Card*> removeCards;
	std::list<Card*> addCards;

	//// 手札に加えるカードと墓地に送るカードを決定
	// 無属性は必ず手札に加える
	addCards.insert(addCards.end(), NoneCards.begin(), NoneCards.end());

	//// 光と闇の数で場合分け
	if (lIndex > dIndex) {
		// 光の方が多いなら、闇を墓地に送る
		removeCards.insert(removeCards.end(), DarknessCards.begin(), DarknessCards.end());
		addCards.insert(addCards.end(), LightCards.begin(), LightCards.end());
	} else if (lIndex < dIndex) {
		// 闇の方が多いなら、光を墓地に送る
		removeCards.insert(removeCards.end(), LightCards.begin(), LightCards.end());
		addCards.insert(addCards.end(), DarknessCards.begin(), DarknessCards.end());
	} else {
		// 同じなら両方手札に加える
		addCards.insert(addCards.end(), LightCards.begin(), LightCards.end());
		addCards.insert(addCards.end(), DarknessCards.begin(), DarknessCards.end());
	}
	std::list<Card*> openCards;
	openCards.insert(openCards.end(), zoneMap[CardZone::Open].begin(), zoneMap[CardZone::Open].end());

	//// 実際にカードを移動

	// 手札に加えるカードを移動
	for (const auto& card : openCards) {
		for (const auto& card2 : addCards) {
			if (card == card2) {
				MoveCard(card, CardZone::Hand);
				break;
			}
		}
	}
	// 手札に加えるカードの移動アニメーション
	std::vector<std::unique_ptr<CardMove>> moves;
	std::unique_ptr<HandCardMove> handMove = std::make_unique<HandCardMove>();
	std::vector<Card*> handCards;
	for (const auto& card : addCards) {
		handCards.push_back(card);
	}
	handMove->Initialize(handCards, 0.3f);
	moves.push_back(std::move(handMove));
	AddCardMove(std::move(moves));

	moves.clear();
	// 墓地に送るカードを移動
	for (const auto& card : removeCards) {
		std::unique_ptr<CardMove> move = std::make_unique<CardMove>();
		move->Initialize(card, CemeteryCardPos(), 0.5f, false);
		moves.push_back(std::move(move));
		MoveCard(card, CardZone::Cemetery);
	}
	AddCardMove(std::move(moves));
}

void CardManager::HandAdjustment() {
	Vector2 pos;
	int i = 0;
	int size = static_cast<int>(zoneMap[CardZone::Hand].size()) - 1;
	for (const auto& card : zoneMap[CardZone::Hand]) {
		if (card->IsCommandMove()) {
			size--;
		}
	}
	for (const auto& card : zoneMap[CardZone::Hand]) {
		if (card->IsCommandMove()) {
			continue;
		}
		pos = HandCardPos(i);
		card->SetNewPos(pos);
		card->SetIsDraw(true);
		i++;
	}
}

void CardManager::ReShuffleDeck() {
	std::vector<Card*> cemeteryCards = zoneMap[CardZone::Cemetery];
	Vector2 pos = Vector2{-100.0, -100.0f};
	runawayGauge_ += 5.0f;
	for (const auto& card : cemeteryCards) {
		card->SetPos(pos);
		card->RessetVariable();
		card->SetIsDraw(false);
		MoveCard(card, CardZone::Deck);
	}
	std::shuffle(zoneMap[CardZone::Deck].begin(), zoneMap[CardZone::Deck].end(), seed);

	std::unique_ptr<CardShuffleMove> shuffleMove = std::make_unique<CardShuffleMove>();
	shuffleMove->Initialize(cemeteryCards, shuffleSE, 1.0f);
	std::vector<std::unique_ptr<CardMove>> moves;
	moves.push_back(std::move(shuffleMove));
	AddCardMove(std::move(moves));

}

void CardManager::ExecutionCard() {
	if (!IsMoveCard()) {
		if (!effectStandby_.empty()) {
			if (effectStandby_.front()->GetType() == CardType::Building) {
				if (effectStandby_.front()->GetZone() == CardZone::Execution) {
					int i = 0;
					for (auto& fi : fieldCardField) {
						if (!fi.isOn) {
							if (zoneMap[CardZone::Hand][holdCardIndex]->IsCostSufficient(lightCost, darknessCost)) {
								Vector2 pos = FieldCardPos(i);
								fi.isOn = true;
								fi.card = effectStandby_.front();
								effectStandby_.front()->SetNewPos(pos);
								effectStandby_.front()->SetIsDraw(true);
								effectStandby_.front()->InitializeDurability();
								MoveCard(zoneMap[CardZone::Hand][holdCardIndex], CardZone::Field);
								break;
							}
						}
						i++;
					}
					effectStandby_.pop();
					return;
				}
			}

			if (effectStandby_.front()->Effect()) {
				if (effectStandby_.front()->GetZone() == CardZone::Execution) {
					MoveCard(effectStandby_.front(), CardZone::Cemetery);
				}
				effectStandby_.pop();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

std::vector<Card*> CardManager::OpenDeck(int num, bool isCommand) {
	std::vector<Card*> result;
	Vector2 pos{640.0f, -160.0f};
	for (int i = 0; i < num; i++) {
		if (zoneMap[CardZone::Deck].empty()) {
			ReShuffleDeck();
		}
		zoneMap[CardZone::Deck].front()->SetPos(pos);
		zoneMap[CardZone::Deck].front()->SetIsDraw(true);
		result.push_back(zoneMap[CardZone::Deck].front());
		if (zoneMap[CardZone::Deck].front()->GetElement() == CardElement::Light) {
			lightCost++;
		} else if (zoneMap[CardZone::Deck].front()->GetElement() == CardElement::Darkness) {
			darknessCost++;
		}
		MoveCard(zoneMap[CardZone::Deck].front(), CardZone::Open);
	}
	if (!isCommand) {
		std::unique_ptr<OpenDeckAlignmentMove> openMove = std::make_unique<OpenDeckAlignmentMove>(); \
			std::vector<std::unique_ptr<CardMove>> moves;
		moves.push_back(std::move(openMove));
		AddCardMove(std::move(moves));
	}
	return result;
}

void CardManager::SetIsSelectCard(bool isSelect) {
	isSelectCard = isSelect;
	endSelectButton->SetIsDraw(isSelect);
	if (!isSelect) {
		selectCards.clear();
	}
}

void CardManager::SetEndSelectButtonColorV(float v) {
	endSelectButton->SetColor({endSelectButtonColor.x * v, endSelectButtonColor.y * v, endSelectButtonColor.z * v, 1.0f});
}

void CardManager::SetEndSelectButtonNormalVector() {
	endSelectButton->SetPosition({640.0f, 650.0f});
}

void CardManager::SetEndSelectButtonHandVector() {
	endSelectButton->SetPosition({640.0f, 360.0f});
}

bool CardManager::IsEndSelectButton() const {
	return endSelectButton->IsOnCollision(Input::GetInstance()->GetMousePos());
}

void CardManager::ShufleCards(std::vector<Card*>& cards) {
	std::shuffle(cards.begin(), cards.end(), seed);
}

void CardManager::MoveCard(Card* card, CardZone cardZone) {

	std::vector<Card*>& zone = zoneMap[card->GetZone()];
	auto it = std::find(zone.begin(), zone.end(), card);
	if (it != zone.end()) {
		zone.erase(it); // target を vector から削除
	}

	card->SetZone(cardZone);
	zoneMap[cardZone].push_back(card);
}

/////////////////////////////////////////////////////////////////

void CardManager::CostTextUpdate() {
	lightCostText->SetText(lightCostText->GetIntToString(lightCost, 2));
	darknessCostText->SetText(darknessCostText->GetIntToString(darknessCost, 2));
}

void CardManager::AllCardLoad(const std::string& file) {
	std::string basePath = file;
	std::vector<CardFile> cardFiles;

	for (const auto& entry : fs::recursive_directory_iterator(basePath)) {
		if (entry.is_regular_file()) {
			auto path = entry.path();
			std::u8string modName8 = path.parent_path().filename().u8string(); // mod名
			std::string modName(modName8.begin(), modName8.end());

			std::u8string fileName8 = path.filename().u8string();
			std::string fileName(fileName8.begin(), fileName8.end());

			cardFiles.push_back({modName, path.string(), fileName});
			std::unique_ptr<CardData> cardData = std::make_unique<CardData>();
			cardData->LoadCardFile(path.string());
			CardDataMap.insert(std::pair(fileName, std::move(cardData)));
		}
	}


}

Vector2 CardManager::GetCardPos(CardZone zone, int index) {
	if (zone == CardZone::Hand) {
		return HandCardPos(index);
	} else if (zone == CardZone::Open) {
		return OpenCardPos(index);
	} else if (zone == CardZone::Cemetery) {
		return CemeteryCardPos();
	} else if (zone == CardZone::Deck) {
		return Vector2(640.0f, -160.0f);
	}

	return Vector2();
}

Vector2 CardManager::HandCardPos(int index) {
	Vector2 pos = {};
	int size = static_cast<int>(zoneMap[CardZone::Hand].size()) - 1;
	pos.x = 640.0f - (size / 2.0f - index) * (cardSizeW + handCardPading);
	pos.y = 720.0f - 80.0f;
	return pos;
}

Vector2 CardManager::OpenCardPos(int index) {
	Vector2 pos = {};
	int size = static_cast<int>(zoneMap[CardZone::Open].size()) - 1;
	pos.x = 640.0f - (size / 2.0f - index) * (cardSizeW + openCardPading);
	pos.y = 140.0f;
	return pos;
}

Vector2 CardManager::CemeteryCardPos() {
	return Vector2(-150.0f, 80.0f);
}

Vector2 CardManager::FieldCardPos(int i) {
	if (i > -1) {
		return {200.0f + i * (cardSizeW * 1.1f + fieldCardPading), 360.0f};
	}
	int index = 0;
	Vector2 pos = {};
	pos.y = 360.0f;
	for (const auto& card : fieldCardField) {
		if (card.isOn) {
			pos.x = 200.0f + index * (cardSizeW * 1.1f + fieldCardPading);
			return pos;
		}
		index++;
	}
	return Vector2(-200.0f, -200.0f);
}
