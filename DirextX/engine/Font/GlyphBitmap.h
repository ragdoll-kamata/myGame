#pragma once
#include <cstdint>
#include <vector>

struct GlyphBitmap {
	uint32_t codepoint;   // 文字
	uint32_t width;       // ビットマップの幅
	uint32_t height;      // ビットマップの高さ
	int32_t  bearingX;    // 基準点からのオフセットX（左側ベアリング）
	int32_t  bearingY;    // 基準点からのオフセットY（上側ベアリング）
	int32_t  advance;     // 次の文字に進む量
	std::vector<uint8_t> pixels; // ビットマップデータ（たとえば8bitグレースケール）
};