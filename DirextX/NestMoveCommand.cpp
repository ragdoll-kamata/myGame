#include "NestMoveCommand.h"
#include "Card.h"

bool NestMoveCommand::Initialize(int index) {
	index_ = index;
    return true;
}

int NestMoveCommand::Execute(Card* card) {
	return 0;
}
