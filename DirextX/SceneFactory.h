#pragma once
#include "AbstractSceneFactory.h"
class SceneFactory : public AbstractSceneFactory {
public:

	std::unique_ptr<BaseScene> CreateScene(std::string name) override;

private:
	
};
