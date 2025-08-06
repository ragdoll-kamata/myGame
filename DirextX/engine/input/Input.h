#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include "WinApp.h"

#include "Vector2.h"

class Input {
public:
	void Initialize(WinApp* winApp);

	void Update();

	////// キーボード //////

	/// <summary>
	/// キーを押した瞬間
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押した瞬間ならtrue</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーを押している
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>押しているならtrue</returns>
	bool PressKey(BYTE keyNumber);

	/// <summary>
	/// キーを離したか瞬間
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>離した瞬間ならtrue</returns>
	bool ReleaseKey(BYTE keyNumber);

	//////　マウス //////

	/// <summary>
	/// マウスボタンを押した瞬間
	/// </summary>
	/// <param name="button">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>押した瞬間ならtrue</returns>
	bool TriggerMouseButton(int button);

	/// <summary>
	/// マウスボタンを押している
	/// </summary>
	/// <param name="button">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>押しているならtrue</returns>
	bool PressMouseButton(int button);

	/// <summary>
	/// マウスボタンを離した瞬間
	/// </summary>
	/// <param name="button">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>離した瞬間ならtrue</returns>
	bool ReleaseMouseButton(int button);

	/// <summary>
	/// マウスの位置を取得
	/// </summary>
	/// <returns>マウス位置</returns>
	const Vector2& GetMousePos() const;

	/// <summary>
	/// マウス移動量の取得(1フレーム分)
	/// </summary>
	/// <returns>移動量</returns>
	const Vector2& GetMouseMove();

	/// <summary>
	/// マウス移動量の長さの取得(1フレーム分)
	/// </summary>
	/// <returns></returns>
	float GetMouseMoveLength() const;

	/// <summary>
	/// ホイールの回転量の取得(1フレーム分)
	/// </summary>
	/// <returns>回転量</returns>
	float GetMouseWheel() const;




	static Input* GetInstance();
	void Finalize();
private:
	// インスタンス
	WinApp* winApp_ = nullptr;

	// キーボード
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE preKey[256] = {};

	// マウス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 preMouseState = {};
	Vector2 mousePos;
	Vector2 mouseMove;

	float mouseWheel = 0.0f;
private:
	static Input* instance;

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = default;
};

