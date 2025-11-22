#pragma once
#include <d3d12.h>
#include <wrl.h>

#include <string>
#include <vector>

#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "GlyphInfo.h"
#include "VertexBuffer.h"
#include "Buffer.h"

class Text {
private:
	struct Tag {
		std::u32string name;                    // 例: "color"
		std::u32string value;                   // 例: "#FF0000"
		size_t startIndex;
		size_t endIndex;
	};

	struct ParsedText {
		std::u32string visibleText;          // タグを除いた表示テキスト
		std::vector<Tag> tags;              // タグ情報
	};
public:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
	enum class TextFormat {
		Right,
		Centor,
		Left,
	};
	struct TextData {
		Matrix4x4 WVP;
		Vector4 color; // RGBA
		Vector2 uvTopLeft;
		Vector2 uvBottomRight;
		uint32_t layerIndex;
		uint32_t isTemp;
	};

	struct CharacterLayout {
		char32_t character;
		Vector2 localPos; // ローカル座標（未補正）
		uint32_t lineIndex = 0;
		const GlyphInfo* glyph;
		Vector4 color = {0.0f, 0.0f, 0.0f, 1.0f}; // RGBA
		float size = 1.0f; // サイズ倍率
		float time = 0.0f;
		bool isTime = false; // タイムタグが適用されているか
	};

	~Text();

	void Initialize(std::u32string text, const Vector2& pos, float maxWidth);

	void Update();

	void Draw();

	void SetText(std::u32string text) {
		text_ = text;
		isLayoutDirty_ = true;
		isFullLayout = false;
	}
	void SetPosition(const Vector2& pos) {
		pos_ = pos;
	}
	Vector2 GetPosition() const {
		return pos_;
	}
	void SetAnchorPoint(const Vector2& anchor) {
		anchorPoint_ = anchor;
		isLayoutDirty_ = true;
	}
	Vector2 GetAnchorPoint() const {
		return anchorPoint_;
	}
	void SetTextFormat(TextFormat format) {
		textFormat_ = format;
		isLayoutDirty_ = true;
	}
	void SetSize(const Vector2& size) {
		size_ = size;
	}
	void CalcFitSize(float width);
	void CalcFitSizeHeight(float height);
	Vector2 GetSize()const {
		return size_;
	}
	void CalcFitSizeOneHeight(float height);
	/// <summary>
	/// 文字間隔設定
	/// </summary>
	/// <param name="padding">文字間隔</param>
	void SetPadding(float padding) {
		padding_ = padding;
		isLayoutDirty_ = true;
	}

	/// <summary>
	/// 最大横幅設定
	/// </summary>
	/// <param name="maxWidth">最大横幅</param>
	void SetMaxWidth(float maxWidth) {
		maxWidth_ = maxWidth;
	}

	static std::u32string GetIntToString(int num, int count);
private:
	static int Number(int num1, int num2, std::vector<int>& digits);

	/// <summary>
	/// テキストレイアウトを行う。
	/// </summary>
	void TextLayout();

	void FullLayout();

	void UpdateLayoutState();

	/// <summary>
	/// タグ付きテキストを解析する。
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	ParsedText ParseTaggedText(const std::u32string& input);

	std::string ExtractAZ09(const std::u32string& input);
	Vector4 HSVtoRGB(float h, float s, float v);

	Vector4 ParseColor(const std::u32string& hex);
	float ParseTime(const std::u32string& timeStr);


	void CreateVertex();
	void CreateTextData();
private:
	std::u32string text_;
	std::u32string nawText_;
	float time = 0.0f;
	int num = 0;

	Vector2 anchorPoint_ = {0.0f, 0.0f};
	Vector2 pos_ = {10.0f, 10.0f};
	Vector2 size_ = {1.0f, 1.0f};

	TextFormat textFormat_ = TextFormat::Left;
	float maxWidth_ = 100.0f;
	float padding_ = 10.0f;
	int instanceNum = 0;


	std::vector<float> textWidths_;
	std::vector<CharacterLayout> layoutChars_;
	float textWidth_ = 0.0f;
	float textHeight_ = 0.0f;

	const uint32_t kMaxInstance = 200;

	Buffer<TextData> textData;
	uint32_t instancingSrvIndex = 0;

	VertexBuffer<VertexData> vertexBuffer_;


	bool isLayoutDirty_ = true;
	bool isFullLayout = false;
};

