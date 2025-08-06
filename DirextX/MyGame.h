#pragma once
#include "Framework.h"
#include "SceneManager.h"
#include "memory"
class MyGame : public Framework
{
private:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	SceneManager* sceneManager_ = nullptr;
};

