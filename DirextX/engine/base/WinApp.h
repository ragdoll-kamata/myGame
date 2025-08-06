#pragma once
#include <Windows.h>
#include <cstdint>
class WinApp
{
public:
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	void Initialize();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	bool ProcessMessage();

	void Finalize() const;

	HINSTANCE GetHInstance() const { return wc.hInstance; }

	HWND GetHwnd() const { return hwnd; }

private:
	WNDCLASS wc{};
	HWND hwnd;
	RECT wrc;
};

