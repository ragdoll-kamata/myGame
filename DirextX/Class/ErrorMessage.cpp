#include "ErrorMessage.h"

ErrorMessage* ErrorMessage::instance = nullptr;

ErrorMessage* ErrorMessage::GetInstance() {
	if (instance == nullptr) {
		instance = new ErrorMessage;
	}
	return instance;
}

void ErrorMessage::Finalize() {
	delete instance;
	instance = nullptr;
}


