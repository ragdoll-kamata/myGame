#pragma once
#include "CardCommand.h"

class VoidCardCommand : public CardCommand {
public:
	CommandType GetCommandType() override {
		return CommandType::Void;
	}
};