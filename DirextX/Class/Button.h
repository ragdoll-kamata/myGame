#pragma once
#include <string>
#include <memory>
#include <Sprite.h>

class Button {
public:

	void Initialize(const Vector2& pos, const Vector2& size, std::string name, Vector4 color);

	void Update();

	void Draw();

	bool IsOnCollision(Vector2 pos);

	void SetPosition(Vector2 pos) { sprite->SetPosition(pos); }

	void SetTextureRect(Vector2 rect);

	void SetIsDraw(bool isDraw) {
		this->isDraw_ = isDraw;
	}

	void SetColor(Vector4 color) {
		sprite->SetColor(color);
	}
	Vector4 GetColor() const {
		return sprite->GetColor();
	}
	void SetAnchorPoint(const Vector2& pos);
	void SetSize(const Vector2& size) {
		sprite->SetSize(size);
	}

private:
	std::unique_ptr<Sprite> sprite;
	bool isDraw_ = false;
	uint32_t TH;

	Vector2 halfSize;
};
