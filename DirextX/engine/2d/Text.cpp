#include "Text.h"
#include <stack>
#include "TextCommon.h"
#include "Transform.h"
#include "MathUtility.h"
#include "DeltaTime.h"

#include "WinApp.h"

using namespace MathUtility;

void Text::Initialize(std::u32string text, const Vector2& pos, float maxWidth) {
	CreateVertex();
	CreateTextData();

	text_ = text;
	pos_ = pos;
	maxWidth_ = maxWidth;

}

void Text::Update() {
	if (isLayoutDirty_) {
		TextLayout(); // 必ず先に呼ぶ
	}

	instanceNum = 0;
	auto* fontLoader = TextCommon::GetInstance()->GetFontLoader();
	auto* atlasMgr = TextCommon::GetInstance()->GetFontAtlasManager();

	Matrix4x4 view = MakeIdentity4x4();
	Matrix4x4 proj = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);

	time += DeltaTime::GetInstance()->GetDeltaTime();
	for (auto& ch : layoutChars_) {
		if (ch.time > time && ch.isTime) {
			break;
		} else {
			if (ch.isTime) {
				time = 0.0f;
				ch.isTime = false; // 時間経過で表示される文字は一度だけ
			}
		}

		const GlyphInfo* info = ch.glyph;
		Vector2 scaleLocal = {ch.localPos.x * size_.x, ch.localPos.y * size_.y};
		Vector2 drawPos = scaleLocal + pos_;
		float baselineY = drawPos.y + fontLoader->GetAscender() * size_.y;
		float charY = baselineY - static_cast<float>(info->bearingY) * size_.y;

		EulerTransform t{
			{float(info->width) * size_.x, float(info->height) * size_.y, 1.0f},
			{0, 0, 0},
			{drawPos.x, charY, 0.0f}
		};

		Matrix4x4 world = MakeAffineMatrix(t.scale, t.rotate, t.translate);
		TextData* tData = textData.GetStruct();
		tData[instanceNum].WVP = world * view * proj;
		tData[instanceNum].color = ch.color; // 色を適用
		tData[instanceNum].uvTopLeft = info->leftTopUv;
		tData[instanceNum].uvBottomRight = info->rightBottomUv;
		tData[instanceNum].layerIndex = info->layerIndex;
		tData[instanceNum].isTemp = info->isTemp;

		instanceNum++;
	}
}

void Text::Draw() {
	if (instanceNum > 0) {
		auto commandList = TextCommon::GetInstance()->GetDirectXCommon()->GetCommandList();
		commandList->IASetVertexBuffers(0, 1, &vertexBuffer_.GetVertexBufferView());
		commandList->SetGraphicsRootDescriptorTable(0, TextCommon::GetInstance()->GetSrvManager()->GetGPUdescriptorHandle(instancingSrvIndex));

		commandList->DrawIndexedInstanced(6, instanceNum, 0, 0, 0);
	}
}

void Text::CalcFitSize(float width) {
	float scale = width / textWidth_;
	size_.x = scale;
	size_.y = scale;
}

std::u32string Text::GetIntToString(int num, int count) {
	int numcopy = num;
	std::vector<int> digits;

	Number(numcopy, 1, digits);

	int digitCount = static_cast<int>(digits.size());

	int zeroCount = count - digitCount;

	std::u32string result;

	for(int i= 0; i < zeroCount; i++) {
		result += U'0';
	}
	std::string tmp = std::to_string(num); // int → std::string
	std::u32string  s(tmp.begin(), tmp.end()); // ASCIIなのでOK
	result += s;

	return result;
}

int Text::Number(int num1, int num2, std::vector<int>& digits) {
	int i = num1;
	if (num1 >= num2 * 10) {
		i = Number(num1, num2 * 10, digits);
		i = i / num2;
	} else {
		i = num1 / num2;
	}
	digits.push_back(i);
	return num1 % num2;
}

void Text::TextLayout() {
	
	if (!isFullLayout) {
		FullLayout();
	} else {
		UpdateLayoutState();
	}
	isLayoutDirty_ = false;
}

void Text::FullLayout() {
	layoutChars_.clear();
	// 一時データ
	std::vector<std::vector<CharacterLayout>> lines;
	std::vector<CharacterLayout> currentLine;

	float lineWidth = 0.0f;
	float lW = 0.0f;
	float lineHeight = TextCommon::GetInstance()->GetFontLoader()->GetHeight();
	float maxLineWidth = 0.0f;

	float preW = 0.0f;

	ParsedText parsedText_ = ParseTaggedText(text_);
	nawText_ = parsedText_.visibleText;
	int i = 0;
	for (char32_t c : nawText_) {
		// 改行文字の場合
		if (c == U'\n') {
			lines.push_back(currentLine);
			maxLineWidth = std::max<float>(maxLineWidth, lineWidth);
			currentLine.clear();
			lineWidth = 0.0f;
			lW = 0.0f;
			preW = 0.0f;
			continue;
		}

		const GlyphInfo* info = TextCommon::GetInstance()->GetFontAtlasManager()->GetOrCreateGlyphInfo(static_cast<uint32_t>(c));



		// 最大横幅を超えた場合
		if (lineWidth + info->width > maxWidth_) {
			lines.push_back(currentLine);
			maxLineWidth = std::max<float>(maxLineWidth, lineWidth);
			currentLine.clear();
			lineWidth = 0.0f;
			lW = 0.0f;
			preW = 0.0f;
		}
		lW += info->width * anchorPoint_.x + preW;
		// 文字のレイアウト
		CharacterLayout layout;
		layout.character = c;
		layout.localPos = {lW, float(lines.size()) * lineHeight + info->height * anchorPoint_.y};
		layout.glyph = info;

		bool isColor = false;
		bool isTime = false;
		for (Tag a : parsedText_.tags) {
			if (a.startIndex <= i && a.endIndex > i) {
				if (!isColor) {
					if (a.name == U"color") {
						layout.color = ParseColor(a.value);
						isColor = true;
					}
				}
				if (a.name == U"size") {

				}
				if (!isTime) {
					if (a.name == U"time") {
						layout.time = ParseTime(a.value);
						layout.isTime = true;
						isTime = true;
					}
				}
			}
		}
		currentLine.push_back(layout);

		preW = (info->width * (1.0f - anchorPoint_.x) + padding_);
		lineWidth += (info->width + padding_);
		i++;
	}

	// 最後の行を追加
	if (!currentLine.empty()) {
		lines.push_back(currentLine);
		maxLineWidth = std::max<float>(maxLineWidth, lineWidth);
	}

	textWidth_ = maxLineWidth;
	textHeight_ = float(lines.size()) * lineHeight;
	Vector2 anchorOffset = {
		-textWidth_ * anchorPoint_.x,
		-textHeight_ * anchorPoint_.y
	};

	// 最終レイアウト確定
	i = 0;
	textWidths_.clear();
	for (auto& line : lines) {
		float lineW = 0.0f;
		for (const auto& ch : line) {
			lineW += ch.glyph->width;
		}
		lineW += static_cast<float>(line.size() - 1) * padding_;
		textWidths_.push_back(lineW);

		lineW *= size_.x;

		float xOffset = 0.0f;
		switch (textFormat_) {
		case TextFormat::Left:   xOffset = 0.0f; break;
		case TextFormat::Centor: xOffset = (textWidth_ - lineW) / 2.0f; break;
		case TextFormat::Right:  xOffset = (textWidth_ - lineW); break;
		}
		
		for (auto& ch : line) {
			CharacterLayout finalCh = ch;
			finalCh.localPos.x += xOffset + anchorOffset.x;
			finalCh.localPos.y += anchorOffset.y;
			finalCh.lineIndex = static_cast<uint32_t>(i);
			layoutChars_.push_back(finalCh);
		}
		i++;
	}
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	VertexData* vertexData = vertexBuffer_.GetVertexData();

	//左上
	vertexData[0].position = {left, top, 0.0f, 1.0f};
	vertexData[0].texcoord = {0.0f, 0.0f};
	//右上
	vertexData[1].position = {right, top, 0.0f, 1.0f};
	vertexData[1].texcoord = {1.0f, 0.0f};
	//左下
	vertexData[2].position = {left, bottom, 0.0f, 1.0f};
	vertexData[2].texcoord = {0.0f, 1.0f};
	//右下
	vertexData[3].position = {right, bottom, 0.0f, 1.0f};
	vertexData[3].texcoord = {1.0f, 1.0f};
	
	isFullLayout = true;
}

void Text::UpdateLayoutState() {
	Vector2 anchorOffset = {
		-textWidth_ * anchorPoint_.x,
		-textHeight_ * anchorPoint_.y
	};

	float lW = 0.0f;
	float lineHeight = TextCommon::GetInstance()->GetFontLoader()->GetHeight();

	float preW = 0.0f;
	int pre = 1;
	for (auto& ch : layoutChars_) {
		if (pre != ch.lineIndex) {
			lW = 0.0f;
			preW = 0.0f;
			pre = ch.lineIndex;
		}
		const GlyphInfo* info = ch.glyph;

		lW += info->width * anchorPoint_.x + preW;

		ch.localPos = {lW, float(ch.lineIndex + 1) * lineHeight + info->height * anchorPoint_.y};



		float xOffset = 0.0f;
		float lineW = textWidths_[ch.lineIndex];
		switch (textFormat_) {
		case TextFormat::Left:   xOffset = 0.0f; break;
		case TextFormat::Centor: xOffset = (textWidth_ - lineW) / 2.0f; break;
		case TextFormat::Right:  xOffset = (textWidth_ - lineW); break;
		}

		ch.localPos.x += xOffset + anchorOffset.x;
		ch.localPos.y += anchorOffset.y;
		
		preW = (info->width * (1.0f - anchorPoint_.x) + padding_);
	}

	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	VertexData* vertexData = vertexBuffer_.GetVertexData();
	//左上
	vertexData[0].position = {left, top, 0.0f, 1.0f};
	vertexData[0].texcoord = {0.0f, 0.0f};
	//右上
	vertexData[1].position = {right, top, 0.0f, 1.0f};
	vertexData[1].texcoord = {1.0f, 0.0f};
	//左下
	vertexData[2].position = {left, bottom, 0.0f, 1.0f};
	vertexData[2].texcoord = {0.0f, 1.0f};
	//右下
	vertexData[3].position = {right, bottom, 0.0f, 1.0f};
	vertexData[3].texcoord = {1.0f, 1.0f};
	isLayoutDirty_ = false;
}

Text::ParsedText Text::ParseTaggedText(const std::u32string& input) {
	ParsedText result;
	std::stack<std::pair<std::u32string, size_t>> tagStack;
	std::stack<std::u32string> tagValueStack;

	size_t i = 0;
	size_t visibleIndex = 0;

	while (i < input.size()) {
		if (input[i] == U'<' && i + 1 < input.size()) {
			size_t tagStart = i;
			size_t tagEnd = input.find(U'>', i);
			if (tagEnd != std::u32string::npos) {
				std::u32string tagContent = input.substr(tagStart + 1, tagEnd - tagStart - 1);
				if (!tagContent.empty() && tagContent[0] == U'/') {
					// 終了タグ（例: </color>）
					std::u32string tagName(tagContent.begin() + 1, tagContent.end());
					if (!tagStack.empty() && tagStack.top().first == tagName) {
						size_t start = tagStack.top().second;
						std::u32string value = tagValueStack.top();
						tagStack.pop();
						tagValueStack.pop();
						result.tags.push_back({tagName, value, start, visibleIndex});
					}
				} else {
					// 開始タグ（例: <color=#FF0000>）
					size_t eqPos = tagContent.find(U'=');
					if (eqPos != std::u32string::npos) {
						std::u32string tagName(tagContent.begin(), tagContent.begin() + eqPos);
						std::u32string tagValue(tagContent.begin() + eqPos + 1, tagContent.end());
						tagStack.push({tagName, visibleIndex});
						tagValueStack.push(tagValue);
					} else {
						// 値なしのタグ（例: <instant>）は必要に応じて対応
					}
				}
				i = tagEnd + 1;
				continue;
			}
		}

		// 通常の文字
		result.visibleText.push_back(input[i]);
		++i;
		++visibleIndex;
	}

	// タグが閉じられていないまま終わるケース
	while (!tagStack.empty()) {
		auto [name, start] = tagStack.top(); tagStack.pop();
		std::u32string value = tagValueStack.top(); tagValueStack.pop();
		result.tags.push_back({name, value, start, result.visibleText.size()});
	}

	return result;
}

std::string Text::ExtractAZ09(const std::u32string& input) {
	std::string result;
	for (char32_t c : input) {
		if ((c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z') || (c >= U'0' && c <= U'9') || c == U',' || c == U'.') {
			result += static_cast<char>(c);
		}
	}
	return result;
}

Vector4 Text::HSVtoRGB(float h, float s, float v) {
	float c = v * s;  // 彩度による変化成分
	float x = c * (1.0f - std::fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
	float m = v - c;

	float r, g, b;

	if (h < 60.0f) {
		r = c; g = x; b = 0.0f;
	} else if (h < 120.0f) {
		r = x; g = c; b = 0.0f;
	} else if (h < 180.0f) {
		r = 0.0f; g = c; b = x;
	} else if (h < 240.0f) {
		r = 0.0f; g = x; b = c;
	} else if (h < 300.0f) {
		r = x; g = 0.0f; b = c;
	} else {
		r = c; g = 0.0f; b = x;
	}

	return Vector4(r + m, g + m, b + m, 1.0f);
}

Vector4 Text::ParseColor(const std::u32string& hex) {
	if (hex.starts_with(U"#")) {		// '#'の次の部分を取り出し、6桁または8桁の16進数を想定
		std::string hexStr = ExtractAZ09(hex);

		if (hexStr.size() == 6 || hexStr.size() == 8) {
			try {
				uint8_t r = static_cast<uint8_t>(std::stoul(hexStr.substr(0, 2), nullptr, 16));
				uint8_t g = static_cast<uint8_t>(std::stoul(hexStr.substr(2, 2), nullptr, 16));
				uint8_t b = static_cast<uint8_t>(std::stoul(hexStr.substr(4, 2), nullptr, 16));
				uint8_t a = (hexStr.size() == 8)
					? static_cast<uint8_t>(std::stoul(hexStr.substr(6, 2), nullptr, 16))
					: 255;

				return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
			} catch (...) {
				return {0.0f, 0.0f, 0.0f, 1.0f};
			}
		}
	}
	if (hex.starts_with(U"rgba(")) {		// 'rgba('の次の部分を取り出し、閉じカッコ除去まで想定
		auto inside = hex.substr(5);
		// ')'があれば削除（無い場合は気にしない）
		if (!inside.empty() && inside.back() == U')') {
			inside.pop_back();
		}
		std::string colorStr = ExtractAZ09(inside);

		// colorStr は "255,0,0,1" のような形式になる想定
		// ','で分割してfloat変換
		std::vector<float> rgba;
		size_t pos = 0;
		while (true) {
			size_t comma = colorStr.find(',', pos);
			std::string token = (comma == std::string::npos) ? colorStr.substr(pos) : colorStr.substr(pos, comma - pos);
			try {
				rgba.push_back(std::stof(token));
			} catch (...) {
				rgba.push_back(0.0f);
			}
			if (comma == std::string::npos) break;
			pos = comma + 1;
		}

		// RGBAそれぞれを0-1に正規化（RGBは255基準、Aはそのまま）
		if (rgba.size() >= 4) {
			return Vector4(rgba[0] / 255.0f, rgba[1] / 255.0f, rgba[2] / 255.0f, rgba[3]);
		}
	}
	if (hex.starts_with(U"hsv(")) {
		auto inside = hex.substr(4);
		if (!inside.empty() && inside.back() == U')') {
			inside.pop_back();
		}
		std::string colorStr = ExtractAZ09(inside);

		std::vector<float> hsv;
		size_t pos = 0;
		while (true) {
			size_t comma = colorStr.find(',', pos);
			std::string token = (comma == std::string::npos) ? colorStr.substr(pos) : colorStr.substr(pos, comma - pos);
			try {
				hsv.push_back(std::stof(token));
			} catch (...) {
				hsv.push_back(0.0f);
			}
			if (comma == std::string::npos) break;
			pos = comma + 1;
		}

		if (hsv.size() >= 3) {
			return  HSVtoRGB(hsv[0], hsv[1], hsv[2]);  // αは1.0固定
		}
	}

	return {0.0f, 0.0f, 0.0f, 1.0f};
}

float Text::ParseTime(const std::u32string& timeStr) {
	std::string str = ExtractAZ09(timeStr);
	try {
		return std::stof(str);
	} catch (...) {
		return 0.0f; // エラー時はデフォルト値
	}
}


void Text::CreateVertex() {
	vertexBuffer_.Initialize(TextCommon::GetInstance()->GetDirectXCommon(),  4);
}

void Text::CreateTextData() {
	textData.Initialize(TextCommon::GetInstance()->GetDirectXCommon(), kMaxInstance);

	instancingSrvIndex = TextCommon::GetInstance()->GetSrvManager()->Allocate();
	TextCommon::GetInstance()->GetSrvManager()->CreateSRVforStructureBuffer(instancingSrvIndex, textData.GetResource(), kMaxInstance, sizeof(TextData));
}