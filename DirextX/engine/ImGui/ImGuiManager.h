#pragma once
class WinApp;
class DirectXCommon;
class SrvManager;
class ImGuiManager
{
public:
	void Initialize(WinApp* winApp, DirectXCommon* directXCommon, SrvManager* srvManager);

	void Begin();

	void End();

	void Draw();

	void Finalize();

private:
	DirectXCommon* directXCommon_ = nullptr;
};

