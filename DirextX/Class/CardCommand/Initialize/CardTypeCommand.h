#pragma once
#include "VoidCardCommand.h"
class CardTypeCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& type);

	ExecuteResult Execute(Card* card) override;
private:
	CardType type_ = CardType::Error;
};

