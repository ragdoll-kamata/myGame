#pragma once
#include "VoidCardCommand.h"
class SetSelectCardIfCommand : public VoidCardCommand {
public:
	bool Initialize(const std::string& functionName);

	ExecuteResult Execute(Card* card) override;
private:
	std::string functionName_;
};

