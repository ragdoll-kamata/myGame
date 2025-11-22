#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"



std::unique_ptr<BaseScene> SceneFactory::CreateScene(std::string name) {

	if (name == "TitleScene") {
		return  std::make_unique<TitleScene>();
	} 
	else if (name == "GameScene") {
		return  std::make_unique<GameScene>();
	}


	return nullptr;
}
