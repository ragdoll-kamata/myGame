#pragma once
#include "VoidCardCommand.h"
class CardNameCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& name);

	ExecuteResult Execute(Card* card) override;
private:
	std::u32string name_;
};

