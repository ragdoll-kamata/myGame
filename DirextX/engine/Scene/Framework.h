#pragma once
#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResourceLeakChecker.h"
#include "ImGuiManager.h"
#include "SrvManager.h"

class Framework
{
public:
	~Framework() = default;


	void Run();

protected:
	virtual void Initialize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual void Finalize();

protected:
	std::unique_ptr<D3DResourceLeakChecker> d3dResourceLeakChecker = nullptr;
	std::unique_ptr <WinApp> winApp_ = nullptr;
	std::unique_ptr <DirectXCommon> directXCommon_ = nullptr;
	std::unique_ptr <SrvManager> srvManager_ = nullptr;
	std::unique_ptr <ImGuiManager> imGuiManager_ = nullptr;

	bool isEnd = false;
};

