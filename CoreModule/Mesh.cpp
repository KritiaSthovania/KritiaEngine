#include "Mesh.h"
#include <fstream>
using namespace KritiaEngine;

KritiaEngine::Mesh::Mesh(const std::vector<std::vector<Vertex>>& vertices, const std::vector<std::vector<unsigned int>>& indices, const std::vector<std::shared_ptr<Material>>& materials) {
    this->submeshVertices = vertices;
    this->submeshIndices = indices;
    submeshMaterials = materials;
}

KritiaEngine::Mesh KritiaEngine::Mesh::Cube() {
    std::vector<float> cubeVertices = GetDefaultCubeVertices();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i < cubeVertices.size(); i += 11) {
        Vertex vertex;
        vertex.Position.x = cubeVertices[i];
        vertex.Position.y = cubeVertices[i + 1];
        vertex.Position.z = cubeVertices[i + 2];
        vertex.Normal.x = cubeVertices[i + 3];
        vertex.Normal.y = cubeVertices[i + 4];
        vertex.Normal.z = cubeVertices[i + 5];
        vertex.Tangent.x = cubeVertices[i + 6];
        vertex.Tangent.y = cubeVertices[i + 7];
        vertex.Tangent.z = cubeVertices[i + 8];
        vertex.TexCoord.x = cubeVertices[i + 9];
        vertex.TexCoord.y = cubeVertices[i + 10];
        vertices.push_back(vertex);
        indices.push_back(i / 11);
    }
    std::vector<std::vector<Vertex>> submeshVertices = std::vector<std::vector<Vertex>>();
    submeshVertices.push_back(vertices);
    std::vector<std::vector<unsigned int>> submeshIndices = std::vector<std::vector<unsigned int>>();
    submeshIndices.push_back(indices);
    std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlinnPhongShader.vs", "./StandardShader/BlinnPhongShader.fs"))));
    material->albedo = Color(1.0f, 1.f, 1.f, 1);
    material->shininess = 32;
    material->renderMode = Material::Opaque;
    material->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/container_diffuse.png"));
    material->specularMap = std::shared_ptr<Texture>(new Texture("./Assets/Textures/container_specular.png"));
    //material->normalMap = std::shared_ptr<Texture>(new Texture("./Assets/Textures/wall/bricks2_normal.jpg"));
    //material->parallaxMap = std::shared_ptr<Texture>(new Texture("./Assets/Textures/wall/bricks2_disp.jpg"));
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(material);
    return Mesh(submeshVertices, submeshIndices, materials);
}

// Tangent may not be correct
std::vector<float> KritiaEngine::Mesh::GetDefaultCubeVertices() {
    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f  // bottom-left        



    };
    std::vector<float> result;
    for (int i = 0; i < sizeof(cubeVertices) / sizeof(float); i++) {
        result.push_back(cubeVertices[i]);
    }
    return result;
}


std::string KritiaEngine::Mesh::Serialize() {
    json json;
    json["Type"] = "Mesh";
    json["Name"] = name;
    json["Path"] = path;
    json["Number of Submeshes"] = submeshSize;
    for (int i = 0; i < submeshSize; i++) {
        json["Submesh" + std::to_string(i)] = SubmeshSerialize(i);
    }
    return json;
}

std::string KritiaEngine::Mesh::SubmeshSerialize(int index) {
    json json;
    json["Number of Vertices"] = submeshVertices.size();
    for (int i = 0; i < submeshVertices.size(); i++) {
        json["Vertex" + std::to_string(i)] = VertexSerialize(submeshVertices[index][i], i);
        json["Material"] = submeshMaterials[i]->Serialize();
    }
    return json;
}

std::string KritiaEngine::Mesh::VertexSerialize(const Vertex& v, int vertexIndex) {
    json json;
    json["Position"] = { v.Position.x, v.Position.y, v.Position.z };
    json["Normal"] = { v.Normal.x, v.Normal.y, v.Normal.z };
    json["Tangent"] = { v.Tangent.x, v.Tangent.y, v.Tangent.z };
    json["TexCoord"] = { v.TexCoord.x, v.TexCoord.y };
    json["Index"] = vertexIndex;
    return json;
}

void KritiaEngine::Mesh::DeserializeFromPath(const std::string& path) {
    std::ifstream instream(path);
    json json = json::parse(instream);
    assert(json["Type"] == "Mesh");
    std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    mesh->name = json["Name"];
    mesh->path = json["Path"];
    mesh->submeshSize = json["Number of Submeshes"];
    mesh->submeshVertices.resize(mesh->submeshSize);
    mesh->submeshVertices.resize(mesh->submeshSize);
    mesh->submeshMaterials.resize(mesh->submeshSize);
    for (int i = 0; i < mesh->submeshSize; i++) {
        // Must first get str to avoid parsing error
        std::string str = json["Submesh" + std::to_string(i)];
        nlohmann::ordered_json submeshJson = json::parse(str);
        mesh->SubmeshDeserialize(submeshJson, i);
        mesh->submeshMaterials[i] = std::shared_ptr<Material>(new Material());
        mesh->submeshMaterials[i]->DeserializeFromJson(json["Material"]);
    }
    instream.close();
}

void KritiaEngine::Mesh::SubmeshDeserialize(const json& json, int index) {
    int numberOfVertices = json["Number of Vertices"];
    for (int i = 0; i < numberOfVertices; i++) {
        std::string str = json["Vertex" + std::to_string(i)];
        nlohmann::ordered_json vertexJson = json::parse(str);
        submeshVertices[index].push_back(Mesh::VertexDeserialize(vertexJson));
        submeshIndices[index].push_back(vertexJson["Index"]);
    }
}


Mesh::Vertex KritiaEngine::Mesh::VertexDeserialize(const json& json) {
    Vertex v;
    v.Position = Vector3(json["Position"][0], json["Position"][1], json["Position"][2]);
    v.Normal = Vector3(json["Normal"][0], json["Normal"][1], json["Normal"][2]);
    v.Tangent = Vector3(json["Tangent"][0], json["Tangent"][1], json["Tangent"[2]]);
    v.TexCoord = Vector2(json["TexCoord"][0], json["TexCoord"][1]);
    return v;
}

bool KritiaEngine::operator<(const std::tuple<Mesh, Material, int>& left, const std::tuple<Mesh, Material, int>& right) {
    return std::get<0>(left).submeshVertices[0].size() < std::get<0>(right).submeshVertices[0].size();
}