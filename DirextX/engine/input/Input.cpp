#include "Input.h"


#include <cassert>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "MathUtility.h"

Input* Input::instance = nullptr;
void Input::Initialize(WinApp* winApp)
{
	// インスタンスゲット
	winApp_ = winApp;

	// DirectInputの初期化
	HRESULT result;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの作成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));

	// データフォーマットを設定
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));

	// 協調レベル（共有モード、フォアグラウンド）
	result = mouse->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	// キーボードの状態を取得
	memcpy(preKey, key, sizeof(key));
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);

	// マウスの状態を取得
	preMouseState = mouseState;
	mouse->Acquire();
	mouse->GetDeviceState(sizeof(mouseState), &mouseState);

	mouseMove.x = static_cast<float>(mouseState.lX);
	mouseMove.y = static_cast<float>(mouseState.lY);

	mouseWheel = static_cast<float>(preMouseState.lZ - mouseState.lZ) / 120.0f;



	
	POINT pos;
	GetCursorPos(&pos); // ← スクリーン座標系
	ScreenToClient(winApp_->GetHwnd(), &pos); // ← ウィンドウ座標系に変換
	mousePos.x = static_cast<float>(pos.x);
	mousePos.y = static_cast<float>(pos.y);
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] && !preKey[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::PressKey(BYTE keyNumber)
{
	if (key[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::ReleaseKey(BYTE keyNumber)
{
	if (!key[keyNumber] && preKey[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::TriggerMouseButton(int button) {
	return (mouseState.rgbButtons[button] & 0x80) && !(preMouseState.rgbButtons[button] & 0x80);
}

bool Input::PressMouseButton(int button) {
	return (mouseState.rgbButtons[button] & 0x80);
}

bool Input::ReleaseMouseButton(int button) {
	return !(mouseState.rgbButtons[button] & 0x80) && (preMouseState.rgbButtons[button] & 0x80);
}

const Vector2& Input::GetMousePos() const {

	return mousePos;
}

const Vector2& Input::GetMouseMove() {
	return mouseMove;
}

inline float Input::GetMouseMoveLength() const {
	return MathUtility::Length(mouseMove);
}

float Input::GetMouseWheel() const {
	return mouseWheel;
}

Input* Input::GetInstance()
{
	if (instance == nullptr) {
		instance = new Input;
	}

	return instance;
}

void Input::Finalize()
{
	delete instance;
	instance = nullptr;
}
