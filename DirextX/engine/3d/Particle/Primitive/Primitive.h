#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <memory>

#include "Vector2.h"
#include "Vector4.h"

#include "DirectXCommon.h"

class Primitive
{
public:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
	virtual ~Primitive() = default;

	virtual void Update();

	void Draw(uint32_t instanceNum);

	Vector2 GetUVScroll() const {
		return UVScroll;
	}
	void SetUVScroll(const Vector2& uvScroll) {
		UVScroll = uvScroll;
	}

public:
	static void SetDirectXCommon(DirectXCommon* directXCommon)
	{
		directXCommon_ = directXCommon;
	}
	static void Finalize() {
		directXCommon_ = nullptr;
	}
	/// <summary>
	/// 板Primitiveの生成
	/// </summary>
	/// <returns>Primitive</returns>
	static std::shared_ptr<Primitive> CreatePlane();

	/// <summary>
	/// リングPrimitiveの生成
	/// </summary>
	/// <param name="division">分割数</param>
	/// <param name="outerRadius">外側の半径</param>
	/// <param name="innerRadius">内側の半径</param>
	/// <returns>Primitive</returns>
	static std::shared_ptr<Primitive> CreateRing(int division, float outerRadius, float innerRadius);

	/// <summary>
	/// 円柱Primitiveの生成
	/// </summary>
	/// <param name="division">分割数</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	/// <returns>Primitive</returns>
	static std::shared_ptr<Primitive> CreateCylinder (int division, float radius, float height);
protected:
	static DirectXCommon* directXCommon_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* indexData = nullptr;
	uint32_t indexCount = 0;

	Vector2 UVScroll = {0.0f, 0.0f};
};

