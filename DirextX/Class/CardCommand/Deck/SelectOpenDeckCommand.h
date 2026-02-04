#pragma once
#include "CardCommand.h"
class SelectOpenDeckCommand : public CardCommand {
public:

	bool Initialize(const std::string& num, const std::string& minSelect, const std::string& maxSelect, const std::string& selectCard, const std::string& notSelectCard);

	// CardCommand を介して継承されました
	ExecuteResult Execute(Card* card) override;
private:
	std::string num_;
	std::string selectCard_;
	std::string notSelectCard_;
	std::string minSelect_;
	std::string maxSelect_;
	int minSelectNum_ = 0;
	int maxSelectNum_ = 0;
	bool isStart_ = false;
	std::vector<Card*> selectCards_;
	std::vector<Card*> notSelectCards_;
	int nawSelectCount_ = 0;
};

