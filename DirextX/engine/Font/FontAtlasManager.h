#pragma once
#include <d3d12.h>
#include <wrl.h>

#include <memory>
#include <vector>
#include <unordered_map>

#include "FontLoader.h"
#include "GlyphInfo.h"


#include "Vector2.h"
#include "Matrix4x4.h"

class DirectXCommon;
class SrvManager;

class FontAtlasManager {
public:
	bool Initialize(DirectXCommon* directXCommon, FontLoader* loader, SrvManager* srvManager, uint32_t atlasWidth, uint32_t atlasHeight);

	const GlyphInfo* GetOrCreateGlyphInfo(uint32_t codepoint);

	bool AddGlyph(uint32_t codepoint);

	void PreDraw();

	void PostDraw();


	void AAAAAAAAA();

	uint32_t GetMainAtlasSrvIndex() const {
		return mainAtlasSrvIndex;
	}

	uint32_t GetTempAtlasSrvIndex() const {
		return tempAtlasSrvIndex;
	}
private:
	void UploadGlyphToTempAtlas(const GlyphBitmap& bitmap, int dstX, int dstY);
	bool CommitTempAtlas(); // 仮アトラスを本アトラスに追加する
	void CopyAtlasLayer(ID3D12Resource* src, uint32_t srcLayer, ID3D12Resource* dst, uint32_t dstLayer);

	UINT Align(UINT value, UINT alignment);
private:
	DirectXCommon* directXCommon_;
	FontLoader* fontLoader_;
	SrvManager* srvManager_;

	uint32_t m_atlasWidth = 0;
	uint32_t m_atlasHeight = 0;
	uint32_t m_currentLayerCount = 0;

	int padding_ = 2;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_tempAtlas;
	uint32_t tempAtlasSrvIndex;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_mainAtlas;
	uint32_t mainAtlasSrvIndex;

	std::unordered_map<uint32_t, GlyphInfo> m_glyphMap;

	std::list< Microsoft::WRL::ComPtr<ID3D12Resource>> uploadBuffers;

	// 簡易パッキング（1行詰め or 棧型でも可）
	uint32_t m_cursorX = 0;
	uint32_t m_cursorY = 0;
	uint32_t m_rowHeight = 0;
};

