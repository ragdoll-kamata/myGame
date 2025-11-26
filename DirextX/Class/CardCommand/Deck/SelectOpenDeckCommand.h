#pragma once
#include "CardCommand.h"
class SelectOpenDeckCommand : public CardCommand {
public:

	bool Initialize(std::string num, std::string minSelect, std::string maxSelect, std::string selectCard, std::string notSelectCard);

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
	int nawSelectCount_ = 0;
};

