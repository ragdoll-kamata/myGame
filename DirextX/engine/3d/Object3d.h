#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include <wrl.h>
#include <d3d12.h>
#include <string>

class Object3dCommon;
class Model;
class Animation;

class Object3d
{
public:
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	struct DirectionalLight {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};
	struct CameraForGPU {
		Vector3 worldPosition;
	};


	void Initialize(Object3dCommon* object3dCommon);

	void Update();

	void Draw();

	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetAnimation(Animation* animation) {
		animation_ = animation;
	}
	const Vector3& GetScale() const { return transform.scale; }
	const Vector3& GetRotate() const { return transform.rotate; }
	const Vector3& GetTranslate() const { return transform.translate; }
	DirectionalLight* GetDirectionalLight() const { return directionalLightResourceData; }


	const void SetScale(const Vector3& scale) { transform.scale = scale; }
	const void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	const void SetTranslate(const Vector3& translate) { transform.translate = translate; }
private:

	void CreateTransformationMatrix();
	void CreateDirectionalLight();
	void CreateCameraForGPU();
	
private:
	Model* model_;
	Animation* animation_;

	Object3dCommon* object3dCommon_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = nullptr;
	TransformationMatrix* wvpData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = nullptr;
	DirectionalLight* directionalLightResourceData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource = nullptr;
	CameraForGPU* cameraResourceData = nullptr;

	EulerTransform transform;
	EulerTransform cameraTransform;
};

