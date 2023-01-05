#include "Scene.h"
#include "../Component/MeshFilter.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "MathStructs.h"
#include "../Component/Light.h"
#include "../Editor/AssetDatabase.h"
#include <algorithm>
#include "../Rendering/RenderingProvider.h"

using namespace KritiaEngine::SceneManagement;
using namespace KritiaEngine::Lighting;
using namespace KritiaEngine;
using namespace KritiaEngineEditor;

std::list<std::shared_ptr<GameObject>> KritiaEngine::SceneManagement::Scene::GetRootGameObjects()
{
    return rootGameObjects;
}

KritiaEngine::SceneManagement::Scene::Scene(const std::string &name)
{
    this->name = name;
    // Create a camera and a light source
    currentCamera = std::shared_ptr<GameObject>(new GameObject("Main Camera"));
    currentCamera->AddComponent<Camera>();
    this->mainLightSource = std::shared_ptr<GameObject>(new GameObject("Main Light Source"));
    std::shared_ptr<Light> light = this->mainLightSource->AddComponent<Light>();
    light->color = Color(1, 1, 1, 1);
    light->type = LightType::Directional;
    light->direction = Vector3::Normalize(Vector3(-1, -1, -1));
}

void KritiaEngine::SceneManagement::Scene::Initialize() {
    InitializeSkybox();
    InitializeCamera();
    InitializeLighting();
    InitializeGameObjects();
}

void KritiaEngine::SceneManagement::Scene::InitializeSkybox() {

}

void KritiaEngine::SceneManagement::Scene::InitializeCamera()
{
    Camera::current = currentCamera->GetComponent<Camera>();
}

void Scene::InitializeLighting() {
    LightingSystem::MainLightSource = this->mainLightSource->GetComponent<Light>();
}

void Scene::InitializeGameObjects() {
    // 暂时不读取文件
    // 生成导入的人物模型：
    //std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    //AssetDatabase::ImportModel("./Assets/Models/rock/rock.obj", mesh);
    ////mesh->submeshMaterials[0]->GPUInstancingEnabled = true;
    //for (int i = 0; i < 5000; i++) {
    //    std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject());
    //    std::shared_ptr<MeshFilter> meshFilter = object->AddComponent<MeshFilter>();
    //    meshFilter->mesh = mesh;
    //    std::shared_ptr<MeshRenderer> renderer = object->AddComponent<MeshRenderer>();
    //    object->Transform()->Scale = Vector3(1, 1, 1);
    //    float randomx = rand() / (double)(RAND_MAX) * 10 - 5;
    //    float randomz = rand() / (double)(RAND_MAX) * 10 - 5;
    //    object->Transform()->Position = Vector3(randomx, -5, randomz);
    //    rootGameObjects.push_back(object);
    //}

    std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject());
    std::shared_ptr<MeshFilter> meshFilter = object->AddComponent<MeshFilter>();
    std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    AssetDatabase::ImportModel("./Assets/Models/gameboard.obj", mesh);
    meshFilter->mesh = mesh;
    //A default material
    std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/TransparentShader.vs", "./StandardShader/TransparentShader.fs"))));
    material->albedo = Color(1.0f, 1.f, 1.f, 1);
    material->shininess = 32;
    material->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/blending_transparent_window.png"));
    material->renderMode = Material::Transparent;
    mesh->submeshMaterials[0] = material;
    std::shared_ptr<MeshRenderer> renderer = object->AddComponent<MeshRenderer>();
    object->Transform()->Scale = Vector3(0.5, 0.5, 0.5);
    object->Transform()->Position = Vector3(0, 0, 0);
    rootGameObjects.push_back(object);

    std::shared_ptr<GameObject> object2 = std::shared_ptr<GameObject>(new GameObject());
    std::shared_ptr<MeshFilter> meshFilter2 = object2->AddComponent<MeshFilter>();
    //mesh->Vertices = vertices_map;
    //mesh->VerticesSize = sizeof(vertices_map);
    std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>(Mesh::Cube());
    //AssetDatabase::ImportModel("./Assets/Models/nanosuit.obj", mesh2);
    meshFilter2->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer2 = object2->AddComponent<MeshRenderer>();
    object2->Transform()->Scale = Vector3(1, 1, 1);
    object2->Transform()->Position = Vector3(0, -2, 0);
    rootGameObjects.push_back(object2);

    std::shared_ptr<GameObject> obj3 = std::shared_ptr<GameObject>(new GameObject());
    std::shared_ptr<MeshFilter> meshFilter3 = obj3->AddComponent<MeshFilter>();
    meshFilter3->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer3 = obj3->AddComponent<MeshRenderer>();
    obj3->Transform()->Scale = Vector3(100, 0.1, 100);
    obj3->Transform()->Position = Vector3(0, -2.57, 0);
    rootGameObjects.push_back(obj3);
}


