#include "WinApp.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void WinApp::Initialize()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	//ウィンドウプロフージャ
	wc.lpfnWndProc = WindowProc;
	//
	wc.lpszClassName = L"Window";
	//
	wc.hInstance = GetModuleHandle(nullptr);
	//
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//
	RegisterClass(&wc);

	//


	//
	wrc = { 0, 0, kClientWidth, kClientHeight };

	//
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//
	hwnd = CreateWindow(
		wc.lpszClassName,
		L"LE3D_04_オジマ_アキト",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	//
	ShowWindow(hwnd, SW_SHOW);
}
//ウィンドウプロフージャ
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	//
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool WinApp::ProcessMessage()
{
	MSG msg{};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize() const
{
	CloseWindow(hwnd);

	CoUninitialize();
}
