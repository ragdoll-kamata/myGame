#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <string>


class SceneManager {
public:
	void Initialize(std::unique_ptr<AbstractSceneFactory> sceneFactory);

	// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetNextScene(std::string sceneName);

	static SceneManager* GetInstance();
	void Finalize();

private:
	static SceneManager* instance;

	std::unique_ptr<BaseScene> activeScene = nullptr;
	std::unique_ptr<BaseScene> nextScene = nullptr;

	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = default;
	SceneManager& operator=(SceneManager&) = default;
};
