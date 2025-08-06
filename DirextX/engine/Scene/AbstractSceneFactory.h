#pragma once
#include <string>
#include <memory>
#include "BaseScene.h"

class AbstractSceneFactory
{
public:
	~AbstractSceneFactory() = default;
	virtual std::unique_ptr<BaseScene> CreateScene(std::string name) = 0;
};

