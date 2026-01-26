#include "Button.h"
#include <algorithm>
#include <TextureManager.h>
using namespace MathUtility;
void Button::Initialize(const Vector2& pos, const Vector2& size, std::string name, Vector4 color) {
	TextureManager::GetInstance()->LoadTexture(name);

	sprite = std::make_unique<Sprite>();
	sprite->Initialize(name);
	sprite->SetAnchorPoint({0.5f, 0.5f});
	sprite->SetSize(size);
	sprite->SetColor(color);
	sprite->SetPosition(pos);
	halfSize = size;
	halfSize /= 2.0f;
}

void Button::Update() {
	if (isDraw_) {
		sprite->Updata();
	}
}

void Button::Draw() {
	if (isDraw_) {
		sprite->Draw();
	}
}

bool Button::IsOnCollision(Vector2 pos) {
	Vector2 anchor = sprite->GetAnchorPoint();
	Vector2 size = sprite->GetSize();
	Vector2 hPos{
		std::clamp(pos.x, sprite->GetPosition().x - size.x * anchor.x, sprite->GetPosition().x + size.x * (1.0f - anchor.x)),
		std::clamp(pos.y, sprite->GetPosition().y - size.y * anchor.y, sprite->GetPosition().y + size.y * (1.0f - anchor.y)),
	};

	hPos.x -= pos.x;
	hPos.y -= pos.y;
	float len = MathUtility::Length(hPos);
	if (len <= 0.0f) {
		return true;
	}
	return false;
}

void Button::SetTextureRect(Vector2 rect) {
	sprite->SetTextureSize(rect);
}

void Button::SetAnchorPoint(const Vector2& pos) {
	sprite->SetAnchorPoint(pos);
}
