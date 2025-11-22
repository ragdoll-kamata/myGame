#pragma once
#include <string>
class ErrorMessage {
public:

	std::u32string GetErrorMessage() const {
		return message;
	}
	void SetMessage(const std::u32string& msg) {
		message = msg;
	}
	int GetErrorLine() const {
		return errorline;
	}
	void SetErrorLine(int line) {
		errorline = line;
	}
public:

	static ErrorMessage* GetInstance();
	void Finalize();

private:
	std::u32string message;
	int errorline = 0;

private:
	static ErrorMessage* instance;

	ErrorMessage() = default;
	~ErrorMessage() = default;
	ErrorMessage(ErrorMessage&) = default;
	ErrorMessage& operator=(ErrorMessage&) = default;
};

