#pragma once
#include "DirectXCommon.h"
class ModelCommon
{
public:
	void Initialize(DirectXCommon* directXCommon);
	DirectXCommon* GetDirectXCommon() const { return directXCommon_; }
private:
	DirectXCommon* directXCommon_;
};

