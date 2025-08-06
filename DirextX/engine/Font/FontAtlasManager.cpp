#include "FontAtlasManager.h"
#include "GlyphBitmap.h"
#include <cassert>

#include "DirectXCommon.h"
#include "SrvManager.h"

bool FontAtlasManager::Initialize(DirectXCommon* directXCommon, FontLoader* loader, SrvManager* srvManager, uint32_t atlasWidth, uint32_t atlasHeight) {
	directXCommon_ = directXCommon;
	fontLoader_ = loader;
	srvManager_ = srvManager;
	m_atlasWidth = atlasWidth;
	m_atlasHeight = atlasHeight;
	m_currentLayerCount = 1; // 最初は1レイヤー

	m_cursorX = 0;
	m_cursorY = 0;
	m_rowHeight = 0;

	m_tempAtlas = directXCommon_->CreateTextTexture2DArray(m_atlasWidth, m_atlasHeight, 1);
	if (m_tempAtlas == nullptr) {
		return false;
	}


	m_mainAtlas = directXCommon_->CreateTextTexture2DArray(m_atlasWidth, m_atlasHeight, 1);
	if (m_mainAtlas == nullptr) {
		return false;
	}

	tempAtlasSrvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforTexture2D(tempAtlasSrvIndex, m_tempAtlas.Get(), DXGI_FORMAT_R8_UNORM, 1);

	mainAtlasSrvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforTexture2DArray(mainAtlasSrvIndex, m_mainAtlas.Get(), DXGI_FORMAT_R8_UNORM, 1, 1);

	return true;
}

const GlyphInfo* FontAtlasManager::GetOrCreateGlyphInfo(uint32_t codepoint) {
	if (!m_glyphMap.contains(codepoint)) {
		if (!AddGlyph(codepoint)) {
			assert(false);
		}
	}
	return &m_glyphMap[codepoint];
}

bool FontAtlasManager::AddGlyph(uint32_t codepoint) {

	// グリフを描画
	GlyphBitmap bitmap = fontLoader_->RenderGlyph(codepoint);
	if (bitmap.pixels.empty() || bitmap.width == 0 || bitmap.height == 0) {
		return false;
	}

	// 改行が必要か判定
	if (m_cursorX + bitmap.width > m_atlasWidth) {
		m_cursorX = 0;
		m_cursorY += m_rowHeight;
		m_rowHeight = 0;
	}

	// 高さオーバーなら仮アトラスが埋まった
	if (m_cursorY + bitmap.height > m_atlasHeight) {
		if (!CommitTempAtlas()) {
			return false;
		}

		// カーソルをリセットして再試行
		m_cursorX = 0;
		m_cursorY = 0;
		m_rowHeight = 0;

		return AddGlyph(codepoint); // 再帰的に再試行
	}

	// ピクセルを仮アトラスに転送する（アップロード）
	UploadGlyphToTempAtlas(bitmap, m_cursorX, m_cursorY);

	// UV計算
	float u0 = static_cast<float>(m_cursorX) / m_atlasWidth;
	float v0 = static_cast<float>(m_cursorY) / m_atlasHeight;
	float u1 = static_cast<float>(m_cursorX + bitmap.width) / m_atlasWidth;
	float v1 = static_cast<float>(m_cursorY + bitmap.height) / m_atlasHeight;

	// GlyphInfo 作成
	GlyphInfo info;
	info.codepoint = codepoint;
	info.leftTopUv = {u0, v0};
	info.rightBottomUv = {u1, v1};
	info.layerIndex = 0;     // 仮アトラスのレイヤーは常に0
	info.isTemp = true;
	info.width = bitmap.width;
	info.height = bitmap.height;
	info.bearingY = bitmap.bearingY;
	

	// 登録
	m_glyphMap[codepoint] = info;

	// カーソル更新
	m_cursorX += bitmap.width + padding_;
	m_rowHeight = std::max<uint32_t>(m_rowHeight, bitmap.height);

	return true;
}

void FontAtlasManager::PreDraw() {
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_tempAtlas.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_mainAtlas.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);
}


void FontAtlasManager::PostDraw() {
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_tempAtlas.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	commandList->ResourceBarrier(1, &barrier);

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_mainAtlas.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	commandList->ResourceBarrier(1, &barrier);
}

void FontAtlasManager::AAAAAAAAA() {
	directXCommon_->AAAAAAAAAAAAAA();
}

void FontAtlasManager::UploadGlyphToTempAtlas(const GlyphBitmap& bitmap, int dstX, int dstY) {
	  // 1. アップロード用リソース（UploadHeap）作成
	const UINT64 uploadPitch = Align(bitmap.width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT); // 256 byte alignment
	const UINT64 uploadSize = uploadPitch * bitmap.height;


	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

	HRESULT hr = directXCommon_->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	);
	assert(SUCCEEDED(hr));

	// 2. データコピー
	uint8_t* mapped = nullptr;
	D3D12_RANGE range = { 0, 0 }; // 書き込みのみ
	uploadBuffer->Map(0, &range, reinterpret_cast<void**>(&mapped));
	for (uint32_t y = 0; y < bitmap.height; ++y) {
		memcpy(mapped + y * uploadPitch, &bitmap.pixels[y * bitmap.width], bitmap.width);
	}
	uploadBuffer->Unmap(0, nullptr);

	// 3. コピー対象情報（Dst = 仮アトラス）
	D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
	dstLoc.pResource = m_tempAtlas.Get();
	dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLoc.SubresourceIndex = 0;

	// 4. コピー元情報（Src = UploadHeap）
	D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
	srcLoc.pResource = uploadBuffer.Get();
	srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLoc.PlacedFootprint.Offset = 0;
	srcLoc.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8_UNORM;
	srcLoc.PlacedFootprint.Footprint.Width = bitmap.width;
	srcLoc.PlacedFootprint.Footprint.Height = bitmap.height;
	srcLoc.PlacedFootprint.Footprint.Depth = 1;
	srcLoc.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(uploadPitch);

	// 5. CopyTextureRegion
	D3D12_BOX dstBox = {};
	dstBox.left = 0;
	dstBox.top = 0;
	dstBox.front = 0;
	dstBox.right = bitmap.width;
	dstBox.bottom = bitmap.height;
	dstBox.back = 1;

	directXCommon_->GetCommandList()->CopyTextureRegion(
		&dstLoc,
		dstX, dstY, 0,
		&srcLoc,
		&dstBox
	);

	// 6. 必要に応じてリソースバリア（仮アトラスをSRVに切り替えるなど）
	// バリア管理は描画直前でまとめてやることもあるため、ここでは省略可能
	uploadBuffers.push_back(uploadBuffer);
}

bool FontAtlasManager::CommitTempAtlas() {
	// 新しいレイヤー数
	uint32_t newLayerCount = m_currentLayerCount + 1;

	// 新しいテクスチャを作成（レイヤー数+1）
	Microsoft::WRL::ComPtr<ID3D12Resource> newAtlas =
		directXCommon_->CreateTextTexture2DArray(m_atlasWidth, m_atlasHeight, newLayerCount);

	// 各レイヤーを旧アトラスから新アトラスへコピー
	for (uint32_t layer = 0; layer < m_currentLayerCount; ++layer) {
		CopyAtlasLayer(m_mainAtlas.Get(), layer, newAtlas.Get(), layer);
	}

	// 仮アトラスを新アトラスの最後のレイヤーへコピー
	CopyAtlasLayer(m_tempAtlas.Get(), 0, newAtlas.Get(), m_currentLayerCount);
	for (auto& [codepoint, glyphInfo] : m_glyphMap) {
		if (glyphInfo.isTemp) {
			glyphInfo.isTemp = false;
			glyphInfo.layerIndex = m_currentLayerCount;
		}
	}

	// mainAtlasを置き換え
	m_mainAtlas = newAtlas;
	m_currentLayerCount = newLayerCount;

	// 仮アトラスをクリア（次回用に再初期化）
	m_tempAtlas = directXCommon_->CreateTextTexture2DArray(m_atlasWidth, m_atlasHeight, 1);

	srvManager_->CreateSRVforTexture2D(tempAtlasSrvIndex, m_tempAtlas.Get(), DXGI_FORMAT_R8_UNORM, 1);

	srvManager_->CreateSRVforTexture2DArray(mainAtlasSrvIndex, m_mainAtlas.Get(), DXGI_FORMAT_R8_UNORM, 1, m_currentLayerCount);

	return true;
}

void FontAtlasManager::CopyAtlasLayer(ID3D12Resource* src, uint32_t srcLayer, ID3D12Resource* dst, uint32_t dstLayer) {
	auto commandList = directXCommon_->GetCommandList();

	   // コピー先
	D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
	dstLoc.pResource = dst;
	dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLoc.SubresourceIndex = dstLayer;

	// コピー元
	D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
	srcLoc.pResource = src;
	srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	srcLoc.SubresourceIndex = srcLayer;

	// Box は nullptr（全体コピー）
	commandList->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);
}

UINT FontAtlasManager::Align(UINT value, UINT alignment) {
	return (value + alignment - 1) & ~(alignment - 1);
}

