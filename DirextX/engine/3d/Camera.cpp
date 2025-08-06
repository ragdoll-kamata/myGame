#include "Camera.h"
#include "WinApp.h"

using namespace MathUtility;

Camera::Camera()
	: transform({ {1.0f,1.0f,1.0f},{ 0.0f,0.0f,0.0f },{ 0.0f,0.0f,-50.0f } })
	, fovY(0.45f)
	, aspectRatio(float(WinApp::kClientWidth) / float(WinApp::kClientHeight))
	, nerClip(0.1f)
	, farClip(100.0f)
	, cameraMatrix(MakeAffineMatrix(transform.scale, transform.rotate, transform.translate))
	, viewMatrix(Inverse(cameraMatrix))
	, projectionMatrix(MakePerspectiveFovMatrix(fovY, aspectRatio, nerClip, farClip))
	, viewProjectionMatrix(viewMatrix * projectionMatrix)
{}

void Camera::Update()
{
	cameraMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nerClip, farClip);
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

Vector3 Camera::GetWorldPosition()
{
	Vector3 result = {
		cameraMatrix.m[3][0],
		cameraMatrix.m[3][1],
		cameraMatrix.m[3][2],
	};

	return result;
}
