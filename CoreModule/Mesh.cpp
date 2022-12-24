#include "Mesh.h"


KritiaEngine::Mesh::Mesh(std::vector<std::vector<Vertex>> vertices, std::vector<std::vector<unsigned int>> indices, std::vector<std::shared_ptr<Material>> materials) {
    this->submeshVertices = vertices;
    this->submeshIndices = indices;
    submeshMaterials = materials;
}

KritiaEngine::Mesh KritiaEngine::Mesh::Cube() {
    std::vector<float> cubeVertices = GetDefaultCubeVertices();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i < cubeVertices.size(); i += 8) {
        Vertex vertex;
        vertex.Position.x = cubeVertices[i];
        vertex.Position.y = cubeVertices[i + 1];
        vertex.Position.z = cubeVertices[i + 2];
        vertex.Normal.x = cubeVertices[i + 3];
        vertex.Normal.y = cubeVertices[i + 4];
        vertex.Normal.z = cubeVertices[i + 5];
        vertex.TexCoord.x = cubeVertices[i + 6];
        vertex.TexCoord.y = cubeVertices[i + 7];
        vertices.push_back(vertex);
        indices.push_back(i / 8);
    }
    std::vector<std::vector<Vertex>> submeshVertices = std::vector<std::vector<Vertex>>();
    submeshVertices.push_back(vertices);
    std::vector<std::vector<unsigned int>> submeshIndices = std::vector<std::vector<unsigned int>>();
    submeshIndices.push_back(indices);
    std::shared_ptr<Material> material2 = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/BlingPhongShader.vs", "./StandardShader/BlingPhongShader.fs"))));
    material2->albedo = Color(1.0f, 1.f, 1.f, 1);
    material2->shininess = 32;
    material2->renderMode = Material::Opaque;
    material2->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/container_diffuse.png"));
    material2->GPUInstancingEnabled = true;
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(material2);
    return Mesh(submeshVertices, submeshIndices, materials);
}

std::vector<float> KritiaEngine::Mesh::GetDefaultCubeVertices() {
    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0, 0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0, 1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        



    };
    std::vector<float> result;
    for (int i = 0; i < sizeof(cubeVertices) / sizeof(float); i++) {
        result.push_back(cubeVertices[i]);
    }
    return result;
}

