#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SrvManager.h"

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* directXCommon, SrvManager* srvManager)
{
	directXCommon_ = directXCommon;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	
	int32_t index = srvManager->Allocate();

	ImGui_ImplDX12_Init(
		directXCommon->GetDevice(),
		static_cast<int>(directXCommon->GetSwapChainResourcesNum()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvManager->GetDescriptorHeap(),
		srvManager->GetCPUdescriptorHandle(index),
		srvManager->GetGPUdescriptorHandle(index)
	);
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXCommon_->GetCommandList());
}

void ImGuiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
