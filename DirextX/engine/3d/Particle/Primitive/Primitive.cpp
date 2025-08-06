#include "Primitive.h"
#include "Plane.h"
#include "Ring.h"
#include "Cylinder.h"

DirectXCommon* Primitive::directXCommon_ = nullptr;
void Primitive::Update() {
	if (UVScroll.x >= 1.0f) {
		UVScroll.x -= 1.0f;
	}
	if (UVScroll.y >= 1.0f) {
		UVScroll.y -= 1.0f;
	}
}
void Primitive::Draw(uint32_t instanceNum) {
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList->IASetIndexBuffer(&indexBufferView);
	commandList->DrawIndexedInstanced(indexCount, instanceNum, 0, 0, 0);
}
std::shared_ptr<Primitive> Primitive::CreatePlane() {
	std::shared_ptr<Plane> plane = std::make_unique<Plane>();
	plane->Initialize();
	return plane;
}

std::shared_ptr<Primitive> Primitive::CreateRing(int division, float outerRadius, float innerRadius) {
	std::shared_ptr<Ring> ring = std::make_unique<Ring>();
	ring->Initialize(division, outerRadius, innerRadius);
	return ring;
}

std::shared_ptr<Primitive> Primitive::CreateCylinder (int division, float radius, float height) {
	std::shared_ptr<Cylinder> cylinder = std::make_unique<Cylinder> ();
	cylinder->Initialize (division, radius, height);
	return cylinder;
}
