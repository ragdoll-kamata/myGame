#include "Model.h"
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "MathUtility.h"

#include "ModelManager.h"

#include <unordered_map>

#include "SkinCluster.h"
#include "Object3dCommon.h"

void Model::Initialize(ModelCommon* modelCommon, const std::string& directoryPath, const std::string& filename, const bool& isAnimation) {
	modelCommon_ = modelCommon;
	isAnimation_ = isAnimation;

	modelData = LoadModelFile(directoryPath, filename);

	CreateVertex();
	CreateIndex();
	CreateMaterial();
}

void Model::Draw() {
	ID3D12GraphicsCommandList* commandList = modelCommon_->GetDirectXCommon()->GetCommandList();
	if (skinCluster_) {
		Object3dCommon::GetInstance()->IfSkinningDraw(true);
		int i = 0;
		for (const auto& meshData : modelData.meshDatas) {
			D3D12_VERTEX_BUFFER_VIEW vertexBufferView[2] = {
				meshData.vertexBufferView,
				skinCluster_->GetInfluenceBufferView(i)
			};
			commandList->IASetVertexBuffers(0, 2, vertexBufferView);
			commandList->IASetIndexBuffer(&meshData.indexBufferView);
			commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData.materials[meshData.materialIndex].textureFilePath));
			commandList->SetGraphicsRootDescriptorTable(5, skinCluster_->GetPaletteSrvHandle());
			commandList->DrawIndexedInstanced(UINT(meshData.indices.size()), 1, 0, 0, 0);
			i++;
		}
	} else {
		Object3dCommon::GetInstance()->IfSkinningDraw(false);
		for (const auto& meshData : modelData.meshDatas) {
			commandList->IASetVertexBuffers(0, 1, &meshData.vertexBufferView);
			commandList->IASetIndexBuffer(&meshData.indexBufferView);
			commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData.materials[meshData.materialIndex].textureFilePath));
			commandList->DrawIndexedInstanced(UINT(meshData.indices.size()), 1, 0, 0, 0);
		}
	}

}

void Model::CreateVertex() {
	for (auto& meshData : modelData.meshDatas) {
		meshData.vertexResource = modelCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(VertexData) * meshData.vertices.size());
		meshData.vertexBufferView.BufferLocation = meshData.vertexResource->GetGPUVirtualAddress();
		meshData.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * meshData.vertices.size());
		meshData.vertexBufferView.StrideInBytes = sizeof(VertexData);

		meshData.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&meshData.vertexData));
		std::memcpy(meshData.vertexData, meshData.vertices.data(), sizeof(VertexData) * meshData.vertices.size());

	}
}

void Model::CreateIndex() {
	for (auto& meshData : modelData.meshDatas) {
		assert(meshData.indices.size() % 3 == 0);
		meshData.indexResource = modelCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(uint32_t) * meshData.indices.size());
		meshData.indexBufferView.BufferLocation = meshData.indexResource->GetGPUVirtualAddress();
		meshData.indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * meshData.indices.size());
		meshData.indexBufferView.Format = DXGI_FORMAT_R32_UINT;

		meshData.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&meshData.indexData));
		std::memcpy(meshData.indexData, meshData.indices.data(), sizeof(uint32_t) * meshData.indices.size());

	}
}

void Model::CreateMaterial() {
	materialResource = modelCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(Material));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData =
		Material{
		.color{1.0f, 1.0f, 1.0f, 1.0f},
		.enableLighting{2},
		.uvTransform{MathUtility::MakeIdentity4x4()},
		.shininess{10.0f},
		.isPhonyReflection{1},
	};
}

Model::ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

	assert(scene->HasMeshes());

	modelData.meshDatas.resize(scene->mNumMeshes);
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		mesh->mName;
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		modelData.meshDatas[meshIndex].materialIndex = mesh->mMaterialIndex;

		modelData.meshDatas[meshIndex].vertices.resize(mesh->mNumVertices);
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			VertexData vertex;
			vertex.position = {position.x, position.y, position.z, 1.0f};
			vertex.normal = {normal.x, normal.y, normal.z};
			vertex.texcoord = {texcoord.x, texcoord.y};
			vertex.position.x *= -1.0f;
			vertex.normal.x *= -1.0f;

			modelData.meshDatas[meshIndex].vertices[vertexIndex] = vertex;
		}

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];

				modelData.meshDatas[meshIndex].indices.push_back(vertexIndex);
			}
		}
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.meshDatas[meshIndex].skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotation;
			bindPoseMatrixAssimp.Decompose(scale, rotation, translate);
			Matrix4x4 bindPoseMatrix = MathUtility::MakeAffineMatrix({scale.x, scale.y, scale.z}, {rotation.x, -rotation.y, -rotation.z, rotation.w}, {-translate.x, translate.y, translate.z});
			jointWeightData.inverseBindPoseMatrix = MathUtility::Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({bone->mWeights[weightIndex].mWeight, meshIndex, bone->mWeights[weightIndex].mVertexId});
			}
		}
	}

	modelData.materials.resize(scene->mNumMaterials);
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFirePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFirePath);

			modelData.materials[materialIndex].textureFilePath = textureFirePath.C_Str();

			TextureManager::GetInstance()->LoadTexture(modelData.materials[materialIndex].textureFilePath);

			modelData.materials[materialIndex].textureIndex =
				TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.materials[materialIndex].textureFilePath);

		}
	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

Node Model::ReadNode(aiNode* node) {
	Node result;

	aiVector3D scale, translate;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scale, rotation, translate);
	result.transform.scale = {scale.x, scale.y, scale.z};
	result.transform.rotate = {rotation.x, rotation.y, rotation.z, rotation.w};
	result.transform.translate = {translate.x, translate.y, translate.z};
	result.localMatrix = MathUtility::MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);


	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t i = 0; i < node->mNumChildren; ++i) {
		result.children[i] = ReadNode(node->mChildren[i]);
	}

	return result;
}

Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}

