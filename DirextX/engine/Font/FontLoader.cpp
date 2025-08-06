#include "FontLoader.h"

#include "FontLoader.h"

FontLoader::FontLoader() {
}

FontLoader::~FontLoader() {
	Shutdown();
}

bool FontLoader::Initialize(std::string filepath, uint32_t pixelSize) {
	if (FT_Init_FreeType(&m_library)) {
		return false;
	}

	if (FT_New_Face(m_library, filepath.c_str(), 0, &m_face)) {
		return false;
	}

	// フォントサイズ設定（pixel単位）
	FT_Set_Pixel_Sizes(m_face, 0, pixelSize);

	return true;
}

void FontLoader::Shutdown() {
	if (m_face) {
		FT_Done_Face(m_face);
		m_face = nullptr;
	}
	if (m_library) {
		FT_Done_FreeType(m_library);
		m_library = nullptr;
	}
}

GlyphBitmap FontLoader::RenderGlyph(uint32_t codepoint) {
	GlyphBitmap result{};

	// 1. グリフをロード
	if (FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER)) {
		// 読み込み失敗
		return result;
	}

	FT_GlyphSlot slot = m_face->glyph;
	FT_Bitmap& bitmap = slot->bitmap;

	// 2. Bitmap情報を取得
	result.codepoint = codepoint;
	result.width = bitmap.width;
	result.height = bitmap.rows;
	result.bearingX = slot->bitmap_left;
	result.bearingY = slot->bitmap_top;
	result.advance = slot->advance.x >> 6; // 1/64ピクセル単位なのでビットシフト

	// 3. ピクセルデータをコピー
	result.pixels.resize(result.width * result.height);

	if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
		// グレースケール（一般的）
		memcpy(result.pixels.data(), bitmap.buffer, result.width * result.height);
	} else {
		// 想定外（例えばモノクロなど）はエラー処理がいるかも
	}

	return result;
}
