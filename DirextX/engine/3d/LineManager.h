#pragma once
#include <DirectXCommon.h>
#include <SrvManager.h>

#include <vector>

#include <Vector3.h>
#include <Vector4.h>

#include <Matrix4x4.h>



class LineManager {
public:
	struct Line {
		Vector3 startPos;
		Vector3 endPos;
		Vector3 startScale;
		Vector3 endScale;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f}; // RGBA color
	};
	struct LineData {
		Matrix4x4 startWVP;
		Matrix4x4 endWVP;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	void Initialize(DirectXCommon* directXCommon, SrvManager* srvManager);

	void Update();

	void Draw();

	void AddLine(const Vector3& startPos, const Vector3& endPos, const Vector3& startScale = {1.0f, 1.0f, 1.0f}, const Vector3& endScale = {1.0f, 1.0f, 1.0f}, const Vector4& color = {1.0f, 1.0f, 1.0f, 1.0f}) {
		lines.push_back({startPos, endPos, startScale, endScale, color});
	}
	void ClearLines() {
		lines.clear();
	}

	
	void CreateRootSignature();
	void CreateGraphicsPipeline();

	void CreateLineData();

	static LineManager* GetInstance();
	void Finalize();
private:
	std::vector<Line> lines;

	

	int lineCount = 0;
	Microsoft::WRL::ComPtr <ID3D12Resource> linesResource;
	LineData* linesData = nullptr;
	uint32_t linesDataSrvIndex = 0;
	const int kMaxLines = 500;

	DirectXCommon* directXCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
private:
	static LineManager* instance;
	LineManager() = default;
	~LineManager() = default;
	LineManager(LineManager&) = default;
	LineManager& operator=(LineManager&) = default;
};

