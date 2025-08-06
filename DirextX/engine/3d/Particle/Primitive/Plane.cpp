#include "Plane.h"
#include "MathUtility.h"
using namespace MathUtility;

Plane::Plane()
{
}

void Plane::Initialize()
{
	CreateVertex();
	CreateIndex();
	//左上
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,0.0f };
	//右上
	vertexData[1].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 1.0f,0.0f };
	//左下
	vertexData[2].position = { -0.5f,0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 0.0f,1.0f };
	//右下
	vertexData[3].position = { 0.5f,0.5f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,1.0f };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 3;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 2;
	indexCount = 6;
}

void Plane::Update() {
	Primitive::Update();
	//左上
	vertexData[0].texcoord = Vector2(0.0f, 0.0f) + UVScroll;
	//右上
	vertexData[1].texcoord = Vector2(1.0f, 0.0f) + UVScroll;
	//左下
	vertexData[2].texcoord = Vector2(0.0f, 1.0f) + UVScroll;
	//右下
	vertexData[3].texcoord = Vector2(1.0f, 1.0f) + UVScroll;
}

void Plane::CreateVertex()
{
	vertexResource = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Plane::CreateIndex()
{
	indexResource = directXCommon_->CreateBufferResource(sizeof(uint32_t) * 6);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}
