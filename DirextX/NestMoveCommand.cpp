#include "NestMoveCommand.h"
#include "Card.h"

bool NestMoveCommand::Initialize(int index) {
	index_ = index;
    return true;
}

ExecuteResult NestMoveCommand::Execute(Card* card) {
	return ExecuteResult::Normal;
}
