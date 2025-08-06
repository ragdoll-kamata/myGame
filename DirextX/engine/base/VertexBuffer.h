#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

#include "DirectXCommon.h"
//class DirectXCommon;
template<typename T>
class VertexBuffer {
public:
	void Initialize(DirectXCommon* directXCommon, uint32_t count);

	T* GetVertexData() const {
		return tmp;
	}
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const {
		return vertexBufferView;
	}
	ID3D12Resource* GetResource()const {
		return vertexResource.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	T* tmp = nullptr;
};

template<typename T>
inline void VertexBuffer<T>::Initialize(DirectXCommon* directXCommon, uint32_t count) {
	vertexResource = directXCommon->CreateBufferResource(sizeof(T) * count);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(T) * count;
	vertexBufferView.StrideInBytes = sizeof(T);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&tmp));

}