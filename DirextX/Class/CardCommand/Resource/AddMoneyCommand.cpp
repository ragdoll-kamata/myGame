#include "AddMoneyCommand.h"
#include "CardManager.h"
#include "ResourceManager.h"

bool AddMoneyCommand::Initialize(const std::string& money) {
	std::unique_ptr<Card> cardptr = std::make_unique<Card>();
	int number = ParseInt(money, cardptr.get());
	if (number < 0) {
		return false;
	}
	money_ = number;
	return true;
}

ExecuteResult AddMoneyCommand::Execute(Card* card) {
	cardManager_->GetResourceManager()->AddMoney(money_);
	return ExecuteResult();
}
