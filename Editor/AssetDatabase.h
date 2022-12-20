#pragma once
#include <pch.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../CoreModule/Mesh.h"

using namespace KritiaEngine;

namespace KritiaEngineEditor {
	static class AssetDatabase {
	public:
		static void ImportModel(std::string path, std::shared_ptr<Mesh> mesh);
	private:
		static void ProcessNode(std::string directory, aiNode* node, const aiScene* scene, std::shared_ptr<Mesh> mesh);
		static void ProcessMesh(std::string directory, aiMesh* mesh, const aiScene* scene, std::shared_ptr<Mesh> meshPtr);
		static std::string loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	};

}

