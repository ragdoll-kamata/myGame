#include "Ring.h"
#include "MathUtility.h"

using namespace MathUtility;

Ring::Ring()
{
}

void Ring::Initialize(uint32_t division, float outerRadius, float innerRadius)
{
	CreateVertex(division);
	CreateIndex(division);

	division_ = division;

	const float radian = 2.0f * MathUtility::kPi / static_cast<float>(division);

	//頂点データの作成
	for (uint32_t i = 0; i <= division; i++) {
		float sin = -std::sin(radian * i);
		float cos = std::cos(radian * i);
		float u = static_cast<float>(i) / static_cast<float>(division);
		vertexData[i * 2 + 0].position = { outerRadius * sin,outerRadius * cos,0.0f,1.0f };
		vertexData[i * 2 + 0].texcoord = { u,0.0f };
		vertexData[i * 2 + 1].position = { innerRadius * sin,innerRadius * cos,0.0f,1.0f };
		vertexData[i * 2 + 1].texcoord = { u,1.0f };
	}
	//インデックスデータの作成
	for (uint32_t i = 0; i < division; i++) {
		indexData[i * 6 + 0] = i * 2 + 0;
		indexData[i * 6 + 1] = i * 2 + 1;
		indexData[i * 6 + 2] = i * 2 + 3;
		indexData[i * 6 + 3] = i * 2 + 0;
		indexData[i * 6 + 4] = i * 2 + 3;
		indexData[i * 6 + 5] = i * 2 + 2;
	}
	indexCount = 6 * division;
}

void Ring::Update() {
	Primitive::Update();
	for (uint32_t i = 0; i <= division_; i++) {
		vertexData[i * 2 + 0].texcoord = Vector2(i / static_cast<float>(division_), 0.0f) + UVScroll;
		vertexData[i * 2 + 1].texcoord = Vector2(i / static_cast<float>(division_), 1.0f) + UVScroll;
	}
}

void Ring::CreateVertex(int division)
{
	int vertexCount = 2 * (division + 1);
	vertexResource = directXCommon_->CreateBufferResource(sizeof(VertexData) * vertexCount);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexCount;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Ring::CreateIndex(int division)
{
	int indexCount = 6 * division;
	indexResource = directXCommon_->CreateBufferResource(sizeof(uint32_t) * indexCount);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * indexCount;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}
