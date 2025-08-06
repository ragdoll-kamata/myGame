#pragma once
#include <cstdint>
#include "Vector2.h"

struct GlyphInfo {
	uint32_t codepoint;     // Unicodeコードポイント
	Vector2  leftTopUv;     // テクスチャ内の左上UV座標
	Vector2  rightBottomUv; // テクスチャ内の右下UV座標
	uint32_t layerIndex;    // テクスチャのレイヤー
	bool isTemp;            // 仮アトラスにいるかどうか
	uint32_t width;       // ビットマップの幅
	uint32_t height;      // ビットマップの高さ
	int32_t bearingY;
};