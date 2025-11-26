#pragma once
#include "VoidCardCommand.h"
class CardExplanationCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& explanation);

	ExecuteResult Execute(Card* card) override;
private:
	std::u32string explanation_;
};

