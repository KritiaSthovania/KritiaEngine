#include "AssetDatabase.h"
#include "../CoreModule/Texture.h"
#include "../CoreModule/Material.h"
#include "../CoreModule/Manager/ResourceManager.h"
#include "EditorApplication.h"
#include "../CoreModule/Utilities.h"
#include <fstream>
using namespace KritiaEngine::Editor;
using namespace KritiaEngine;
using namespace KritiaEngine::Manager;

void KritiaEngine::Editor::AssetDatabase::ImportAsset(const std::string& path) {
    // 3D Models
    if (path.ends_with(".obj") || path.ends_with(".fbx")) {
        std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
        ImportModel(path, mesh);
        // in windows use \\ as separator
        std::string name = path.substr(path.find_last_of('\\') + 1, path.find_last_of('.') - path.find_last_of('\\') - 1);
        mesh->name = name;
        mesh->path = path;
        mesh->isPrimitive = false;
        mesh->submeshSize = mesh->submeshVertices.size();
        mesh->SerializeToFile();
        ResourceManager::meshes[mesh->guid] = mesh;
    } 
    // Textures
    else if (path.ends_with(".jpg") || path.ends_with(".png")) {
        std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
        std::string name = path.substr(path.find_last_of('\\') + 1, path.find_last_of('.') - path.find_last_of('\\') - 1);
        tex->name = name;
        tex->path = path;
        tex->SerializeToFile();
        ResourceManager::textures[tex->guid] = tex;
    }
}

void KritiaEngine::Editor::AssetDatabase::ImportModel(const std::string& path, const std::shared_ptr<Mesh>& mesh) {
	Assimp::Importer importer;
	// Change primitive to triangles, flip UV coordinates, generate normals.
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |  aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    std::string directory = path.substr(0, path.find_last_of('\\'));
    std::vector<Bound> submeshBounds = std::vector<Bound>();
    ProcessNode(directory, scene->mRootNode, scene, mesh, submeshBounds);
    float minx = FLT_MAX, miny = FLT_MAX, minz = FLT_MAX, maxx = FLT_MIN, maxy = FLT_MIN, maxz = FLT_MIN;
    for (Bound& b : submeshBounds) {
        if (b.GetMin().x < minx) {
            minx = b.GetMin().x;
        }
        if (b.GetMax().x > maxx) {
            maxx = b.GetMax().x;
        }
        if (b.GetMin().y < miny) {
            miny = b.GetMin().y;
        }
        if (b.GetMax().y > maxy) {
            maxy = b.GetMax().y;
        }
        if (b.GetMin().z < minz) {
            minz = b.GetMin().z;
        }
        if (b.GetMax().z > maxz) {
            maxz = b.GetMax().z;
        }
    }
    Vector3 min = Vector3(minx, miny, minz);
    Vector3 max = Vector3(maxx, maxy, maxz);
    mesh->bound.center = 0.5 * (min + max);
    mesh->bound.size = max - min;
    mesh->submeshSize = mesh->submeshVertices.size();
}

void KritiaEngine::Editor::AssetDatabase::ProcessNode(const std::string& directory, aiNode* node, const aiScene* scene, const std::shared_ptr<Mesh>& mesh, std::vector<Bound>& submeshBounds) {
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        Bound bound;
        bound.center = 0.5 * Vector3((aiMesh->mAABB.mMin + aiMesh->mAABB.mMax).x, (aiMesh->mAABB.mMin + aiMesh->mAABB.mMax).y, (aiMesh->mAABB.mMin + aiMesh->mAABB.mMax).z);
        bound.size = Vector3((aiMesh->mAABB.mMax - aiMesh->mAABB.mMin).x, (aiMesh->mAABB.mMax - aiMesh->mAABB.mMin).y, (aiMesh->mAABB.mMax - aiMesh->mAABB.mMin).z);
        submeshBounds.push_back(bound);
        ProcessMesh(directory, aiMesh, scene, mesh);
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(directory, node->mChildren[i], scene, mesh, submeshBounds);
    }
}

void KritiaEngine::Editor::AssetDatabase::ProcessMesh(const std::string& directory, aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Mesh>& meshPtr) {
    std::vector<KritiaEngine::Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string diffuseMapPath;
    std::string specularMapPath;
    std::string normalMapPath;
    //std::vector<KritiaEngine::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        KritiaEngine::Mesh::Vertex vertex;
        vertex.Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // 处理顶点位置、法线和纹理坐标
        vertex.Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        // if there are texcoords
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoord = Vector2();
        }
        vertices.push_back(vertex);
    }
    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    aiMaterial* aiMaterial = nullptr;
    // 处理材质
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
        diffuseMapPath = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        specularMapPath = loadMaterialTextures(aiMaterial, aiTextureType_SPECULAR, "texture_specular");
        normalMapPath = loadMaterialTextures(aiMaterial, aiTextureType_HEIGHT, "texture_normal");
    }
    meshPtr->submeshVertices.push_back(vertices);
    meshPtr->submeshIndices.push_back(indices);
    if (aiMaterial != nullptr) {
        std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material(aiMaterial->GetName().C_Str()));
        material->albedo = Color(1.0f, 1.f, 1.f, 1);
        material->shininess = 32;
        if (diffuseMapPath != "") {
            material->mainTexture = std::shared_ptr<Texture>(new Texture(directory + "/" + diffuseMapPath));
        }
        if (specularMapPath != "") {
            material->specularMap = std::shared_ptr<Texture>(new Texture(directory + "/" + specularMapPath));
        }
        if (normalMapPath != "") {
            material->normalMap = std::shared_ptr<Texture>(new Texture(directory + "/" + normalMapPath));
        }
        if (material->name == "") {
            material->name = "Submaterial" + std::to_string(meshPtr->submeshMaterials.size());
        }
        material->path = directory + "/" + material->name;
        meshPtr->submeshMaterials.push_back(material);
    }

}

std::string KritiaEngine::Editor::AssetDatabase::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::string texturePath;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        texturePath = str.C_Str();
    }
    return texturePath;
}


