#pragma once
#include "VoidCardCommand.h"
class SetCardCharacteristicsCommand : public VoidCardCommand {
	public:
	bool Initialize(const std::vector<std::string>& characteristics);
	ExecuteResult Execute(Card* card) override;
private:
	std::vector<CardCharacteristics> characteristics_;
};

