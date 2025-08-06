#include "Sprite.h"
#include "SpriteCommon.h"
#include "Transform.h"
#include "TextureManager.h"

using namespace MathUtility;

void Sprite::Initialize(SpriteCommon* spriteCommon, std::string textureFilePath)
{
	spriteCommon_ = spriteCommon;

	CreateVertex();
	CreateIndex();
	CreateMaterial();
	CreateTransformationMatrix();
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
	textureName = textureFilePath;


	AdjustTextureSize();
}

void Sprite::Updata()
{
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}
	const  DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetadata(textureName);
	float tex_left = textureLeftTop_.x / metadata.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float tex_top = textureLeftTop_.y / metadata.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	//左上
	vertexData[0].position = { left,top,0.0f,1.0f };
	vertexData[0].texcoord = { tex_left,tex_top };
	//右上
	vertexData[1].position = { right,top,0.0f,1.0f };
	vertexData[1].texcoord = { tex_right,tex_top };
	//左下
	vertexData[2].position = { left,bottom,0.0f,1.0f };
	vertexData[2].texcoord = { tex_left,tex_bottom };
	//右下
	vertexData[3].position = { right,bottom,0.0f,1.0f };
	vertexData[3].texcoord = { tex_right,tex_bottom };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 3;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 2;


	EulerTransform transform{ {size_.x,size_.y,0.0f} ,{0.0f,0.0f,rotation_},{position_.x,position_.y,0.0f} };
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
	*transformationMatrixData = TransformationMatrix(worldViewProjectionMatrix);


}

void Sprite::Draw()
{
	ID3D12GraphicsCommandList* commandList = spriteCommon_->GetDirectXCommon()->GetCommandList();

	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList->IASetIndexBuffer(&indexBufferView);
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(textureName));

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::CreateVertex()
{
	vertexResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Sprite::CreateIndex()
{
	indexResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}

void Sprite::CreateMaterial()
{
	materialResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(Material));

	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData =
		Material{
			.color{ 1.0f, 1.0f, 1.0f, 1.0f },
			.uvTransform{MakeIdentity4x4()},
		};

	materialData->uvTransform = MakeIdentity4x4();
}

void Sprite::CreateTransformationMatrix()
{
	transformationMatrixResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	*transformationMatrixData = TransformationMatrix({ MakeIdentity4x4() });
}

void Sprite::AdjustTextureSize()
{
	const  DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetadata(textureName);
	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);
	size_.x = static_cast<float>(metadata.width);
	size_.y = static_cast<float>(metadata.height);
}
