#pragma once
#include "MathUtility.h"
#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>
#include <string>

#include "Buffer.h"

class SpriteCommon;



class Sprite
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
	};
	struct TransformationMatrix {
		Matrix4x4 WVP;
	};

	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);

	void Updata();

	void Draw();

	const Vector2& GetPosition() const { return position_; }
	void SetPosition(const Vector2& position) { position_ = position; }

	float GetRotation() const { return rotation_; }
	void SetRotation(float rotation) { rotation_ = rotation; }

	const Vector4& GetColor() const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	const Vector2& GetSize() const { return size_; }
	void SetSize(const Vector2& size) { size_ = size; }

	const Vector2& GetAnchorPoint() const { return anchorPoint_; }
	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }


	bool GetIsFlipX() const { return isFlipX_; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }

	bool GetIsFlipY() const { return isFlipY_; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }

	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }
	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	const Vector2& GetTextureSize() const { return textureSize_; }
	void SetTextureSize_(const Vector2& textureSize) { textureSize_ = textureSize; }

private:

	void CreateVertex();

	void CreateIndex();

	void CreateMaterial();

	void CreateTransformationMatrix();

	void AdjustTextureSize();

	uint32_t textureIndex = 0;
	std::string textureName;

	Vector2 position_ = { 0.0f,0.0f };
	float rotation_ = 0.0f;
	Vector2 size_ = { 100.0f,100.0f };
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	bool isFlipX_ = false;
	bool isFlipY_ = false;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { 100.0f,100.0f };

	SpriteCommon* spriteCommon_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* indexData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformationMatrix* transformationMatrixData = nullptr;

	Buffer<VertexData> a;
};

