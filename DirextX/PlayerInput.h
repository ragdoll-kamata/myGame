#pragma once
#include <functional>
#include <unordered_map>

#include "Input.h"

#include "TrunState.h"

class CardManager;
class ShopManager;
class UIManager;
class ResourceManager;

class PlayerInput {
public:
	void Initialize(CardManager* cardManager, ShopManager* shopManager, UIManager* uiManager, ResourceManager* resourceManager);

	void Update(TurnState& turnState);

private:
	void MainTurnUpdate(TurnState& turnState);
	void ShopTurnUpdate(TurnState& turnState);


	bool TryExecution(Vector2 mousePos, int holdCardIndex);

	bool TryBuilding(Vector2 mousePos, int holdCardIndex);

private:
	CardManager* cardManager_ = nullptr;
	ShopManager* shopManager_ = nullptr;
	UIManager* uiManager_ = nullptr;
	ResourceManager* resourceManager_ = nullptr;
	Input* input_ = nullptr;

	std::unordered_map<TurnState, std::function<void(TurnState&)>> turnMap{
		{TurnState::Main, [&](TurnState& i) { return MainTurnUpdate(i); }},
		{TurnState::Shop, [&](TurnState& i) { return ShopTurnUpdate(i); }},
	};

};

