#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

#include "DirectXCommon.h"

class IndexBuffer {
public:
	void Initialize(DirectXCommon* directXCommon, uint32_t count);

	uint32_t* GetIndexData() const {
		return index;
	}
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const {
		return indexBufferView;
	}
	ID3D12Resource* GetResource()const {
		return indexResource.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* index = nullptr;
};

