#pragma once
#include <map>

#include "ModelCommon.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <Node.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class SkinCluster;

class Model
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex;
	};
	struct VertexWeightData {
		float weight;
		uint32_t meshIndex;
		uint32_t vertexIndex;
	};
	struct JointWeightData {
		Matrix4x4 inverseBindPoseMatrix;
		std::vector<VertexWeightData> vertexWeights;
	};
	struct MeshData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		std::map<std::string, JointWeightData> skinClusterData;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		VertexData* vertexData;
		uint32_t materialIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		uint32_t* indexData = nullptr;
	};
	struct ModelData {
		std::vector<MeshData> meshDatas;
		std::vector<MaterialData> materials;
		Node rootNode;
	};
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
		int32_t isPhonyReflection;
	};



	void Initialize(ModelCommon* modelCommonconst, const std::string& directoryPath, const std::string& filename, const bool& isAnimation);

	void Draw();

	Node* GetRootNode() {
		return &modelData.rootNode;
	}

	Material* GetMaterialData() const { return materialData; }

	const ModelData* GetModelData() const {
		return &modelData;
	}

	void SetSkinCluster(SkinCluster* skinCluster) {
		skinCluster_ = skinCluster;
	}

private:

	void CreateVertex();

	void CreateIndex();

	void CreateMaterial();

	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);


private:
	ModelCommon* modelCommon_;

	SkinCluster* skinCluster_ = nullptr;

	ModelData modelData;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData = nullptr;

	bool isAnimation_ = false;
};

