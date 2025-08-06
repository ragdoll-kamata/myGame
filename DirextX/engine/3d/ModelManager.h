#pragma once
#include <map>
#include <string>
#include <memory>
#include "ModelCommon.h"

class DirectXCommon;
class Model;
class ModelManager
{
public:
	void Initialize(DirectXCommon* directXCommon);

	void LoadModel(const std::string& filePath, const bool& isAnimation = false);

	Model* FindModel(const std::string& filePath);

	void SetDirectoryPath(std::string& directoryPath) {
		directoryPath_ = directoryPath;
	}
	std::string GetDirectoryPath() {
		return directoryPath_;
	}


	static ModelManager* GetInstance();
	void Finalize();
private:
	std::map<std::string, std::unique_ptr<Model>> models;

	ModelCommon* modelCommon = nullptr;

	std::string directoryPath_;

private:
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = default;
	ModelManager& operator=(ModelManager&) = default;

};

