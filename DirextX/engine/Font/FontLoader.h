#pragma once

#include <vector>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "GlyphBitmap.h"

class FontLoader {
public:
	FontLoader();
	~FontLoader();

	bool Initialize(std::string filepath, uint32_t pixelSize);
	void Shutdown();

	GlyphBitmap RenderGlyph(uint32_t codepoint);

	float GetHeight() const {
		return static_cast<float>(m_face->size->metrics.height >> 6);
	}
	float GetAscender() const {
		return static_cast<float>(m_face->size->metrics.ascender >> 6);
	}
	float GetDescender() const {
		return static_cast<float>(m_face->size->metrics.descender >> 6);
	}
	float GetMaxAdvance() const {
		return static_cast<float>(m_face->size->metrics.max_advance >> 6);
	}

private:
	FT_Library m_library = nullptr;
	FT_Face    m_face = nullptr;
};