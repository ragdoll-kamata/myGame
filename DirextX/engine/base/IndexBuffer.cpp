#include "IndexBuffer.h"

void IndexBuffer::Initialize(DirectXCommon* directXCommon, uint32_t count) {
	indexResource = directXCommon->CreateBufferResource(sizeof(uint32_t) * count);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * count;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&index));
}