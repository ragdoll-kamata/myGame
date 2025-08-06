#pragma once
#include "Camera.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>


class CameraManager {
public:

	void Update();

	/// <summary>
	/// 新しいカメラの作成
	/// </summary>
	/// <param name="name">カメラ名</param>
	void CreateNewCamera(std::string name);

	bool IsExistCamera(std::string name);

	/// <summary>
	/// activeCameraの変更
	/// </summary>
	/// <param name="name">カメラ名</param>
	void SetActiveCamera(std::string name);
	/// <summary>
	/// activeCameraの取得
	/// </summary>
	/// <returns></returns>
	Camera* GetActiveCamera() const { return activeCamera; }

	/// <summary>
	/// 入れたカメラ名のポインタを返す関数(カメラ名が無い場合、assertされます)
	/// </summary>
	/// <param name="name">カメラ名</param>
	/// <returns></returns>
	Camera* GetCamera(std::string name);

	Matrix4x4 GetCameraViewProjectionMatrix() const { return activeCamera->GetViewProjectionMatrix(); }

	/// <summary>
	/// instanceの取得(デフォルトでカメラ名[default]がactiveCameraに設定されています)
	/// </summary>
	/// <returns></returns>
	static CameraManager* GetInstance();
private:
	void Initialize();

private:
	static CameraManager* instance;

	std::unordered_map<std::string, std::unique_ptr<Camera>> cameras;
	Camera* activeCamera = nullptr;


	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = default;
	CameraManager& operator=(CameraManager&) = default;
};