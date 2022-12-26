#pragma once
#include <pch.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../CoreModule/Mesh.h"

using namespace KritiaEngine;

namespace KritiaEngineEditor {
	class AssetDatabase {
	public:
		static void ImportModel(const std::string& path, const std::shared_ptr<Mesh>& mesh);
	private:
		static void ProcessNode(const std::string& directory, aiNode* node, const aiScene* scene, const std::shared_ptr<Mesh>& mesh);
		static void ProcessMesh(const std::string& directory, aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Mesh>& meshPtr);
		static std::string loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

	};

}

