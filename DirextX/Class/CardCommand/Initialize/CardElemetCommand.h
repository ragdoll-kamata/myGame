#pragma once
#include "VoidCardCommand.h"
class CardElemetCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& element);

	ExecuteResult Execute(Card* card) override;
private:
	CardElement element_;
};

