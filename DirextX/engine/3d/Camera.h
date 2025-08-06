#pragma once
#include "MathUtility.h"
#include "Transform.h"


class Camera {
public:

	Camera();

	void Update();

	Matrix4x4 GetWorldTransform() const { return cameraMatrix; }

	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix; }

	Vector3 GetPosition() const { return transform.translate; }
	void SetPosition(const Vector3& pos) { transform.translate = pos; }


	Vector3 GetWorldPosition();
	
private:
	EulerTransform transform;
	float fovY;
	float aspectRatio;
	float nerClip;
	float farClip;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;
};