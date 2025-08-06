#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

#include "DirectXCommon.h"
//class DirectXCommon;
template<typename T>
class Buffer {
public:
	void Initialize(DirectXCommon* directXCommon, uint32_t count);

	T* GetStruct() const {
		return tmp;
	}
	ID3D12Resource* GetResource()const {
		return resource.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	T* tmp = nullptr;
};

template<typename T>
inline void Buffer<T>::Initialize(DirectXCommon* directXCommon, uint32_t count) {

	resource = directXCommon->CreateBufferResource(sizeof(T) * count);

	resource->Map(0, nullptr, reinterpret_cast<void**>(&tmp));
	
}
