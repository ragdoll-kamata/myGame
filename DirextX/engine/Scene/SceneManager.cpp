#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;
void SceneManager::Initialize() { 
	sceneFactory = std::make_unique<SceneFactory>();
}

void SceneManager::Update() {
	if (nextScene) {
		if (activeScene) {
			activeScene.reset();
		}

		activeScene = std::move(nextScene);
		nextScene = nullptr;
		activeScene->Initialize();
	}
	activeScene->Update();
}

void SceneManager::Draw() {
	activeScene->Draw();
}

void SceneManager::SetNextScene(std::string sceneName) {
	nextScene = sceneFactory->CreateScene(sceneName);
}

SceneManager* SceneManager::GetInstance() {
	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Finalize() { 
	delete instance;
	instance = nullptr;
}
