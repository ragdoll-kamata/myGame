#include "Object3d.h"
#include "Object3dCommon.h"
#include "MathUtility.h"
#include "Model.h"
#include "ModelManager.h"
#include "CameraManager.h"

#include "Animation.h"

using namespace MathUtility;
void Object3d::Initialize(Object3dCommon* object3dCommon)
{
	object3dCommon_ = object3dCommon;

	CreateTransformationMatrix();
	CreateDirectionalLight();
	CreateCameraForGPU();

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };

}

void Object3d::Update()
{
	if(animation_) {
		Node rootNode = model_->GetModelData()->rootNode;
		/*Vector3 trans = animation_->TranslateCalculateValue(rootNode.name);
		Vector4 rota = animation_->RotateCalculateValue(rootNode.name);
		Vector3 sca = animation_->ScaleCalculateValue(rootNode.name);

		Matrix4x4 localMatrix = MakeAffineMatrix(sca, rota, trans);*/

		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		wvpData->WVP =  worldMatrix * CameraManager::GetInstance()->GetCameraViewProjectionMatrix();
		wvpData->World =  worldMatrix;
	} else {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		wvpData->WVP = model_->GetModelData()->rootNode.localMatrix * worldMatrix * CameraManager::GetInstance()->GetCameraViewProjectionMatrix();
		wvpData->World = model_->GetModelData()->rootNode.localMatrix * worldMatrix;
	}
	

	cameraResourceData->worldPosition = CameraManager::GetInstance()->GetActiveCamera()->GetPosition();
}

void Object3d::Draw()
{
	ID3D12GraphicsCommandList* commandList = object3dCommon_->GetDirectXCommon()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());

	if (model_) {
		model_->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::CreateTransformationMatrix()
{
	wvpResource = object3dCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = TransformationMatrix({ MathUtility::MakeIdentity4x4() });
}

void Object3d::CreateDirectionalLight()
{
	directionalLightResource = object3dCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(DirectionalLight));

	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightResourceData));

	*directionalLightResourceData = DirectionalLight(
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 0.0f,-1.0f,0.0f },
		1.0f
	);
}

void Object3d::CreateCameraForGPU()
{
		cameraResource = object3dCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(CameraForGPU));

		cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraResourceData));

		*cameraResourceData = CameraForGPU(
			{ 1.0f,1.0f,1.0f }
		);

}
