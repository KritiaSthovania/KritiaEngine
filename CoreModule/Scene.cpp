#include "Scene.h"
#include "../Component/MeshFilter.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "MathStructs.h"
#include "../Component/Light.h"
#include "../Editor/AssetDatabase.h"
#include "../Rendering/RenderingProvider.h"
#include "../Editor/EditorApplication.h"
#include <fstream>

using namespace KritiaEngine::SceneManagement;
using namespace KritiaEngine::Lighting;
using namespace KritiaEngine;
using namespace KritiaEngine::Editor;
using json = nlohmann::ordered_json;



std::list<std::shared_ptr<GameObject>> KritiaEngine::SceneManagement::Scene::GetRootGameObjects()
{
    return rootGameObjects;
}

KritiaEngine::SceneManagement::Scene::Scene(const std::string &name, const std::string& path)
{
    this->name = name;
    this->path = path;
}

void KritiaEngine::SceneManagement::Scene::Initialize() {
    std::ifstream input(path);
    if (input.good()) {
        // An serialized scene
        input.close();
        DeserializeFromPath(path);

    } else {
        input.close();
        // A new scene;
        InitializeCamera();
        InitializeLighting();
    }
    //InitializeGameObjects();
}

void KritiaEngine::SceneManagement::Scene::InitializeCamera()
{
    std::shared_ptr<GameObject> mainCamera = std::shared_ptr<GameObject>(new GameObject("Main Camera"));
    currentCamera = mainCamera;
    currentCamera->AddComponent<Camera>();
    rootGameObjects.push_back(mainCamera);
    Camera::current = currentCamera->GetComponent<Camera>();
}

void Scene::InitializeLighting() {
    std::shared_ptr<GameObject> mainLightSource = std::shared_ptr<GameObject>(new GameObject("Main Light Source"));
    this->mainLightSource = mainLightSource;
    std::shared_ptr<Light> light = this->mainLightSource->AddComponent<Light>();
    light->color = Color(1, 1, 1, 1);
    light->Transform()->forward = Vector3::Normalize(Vector3(-1, -1, 1));
    rootGameObjects.push_back(mainLightSource);
    LightingSystem::GetMainLightSource() = this->mainLightSource->GetComponent<Light>();
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
    //    object->Transform()->scale = Vector3(1, 1, 1);
    //    float randomx = rand() / (double)(RAND_MAX) * 10 - 5;
    //    float randomz = rand() / (double)(RAND_MAX) * 10 - 5;
    //    object->Transform()->position = Vector3(randomx, -5, randomz);
    //    rootGameObjects.push_back(object);
    //}

    //std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject());
    //std::shared_ptr<MeshFilter> meshFilter = object->AddComponent<MeshFilter>();
    //std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    //AssetDatabase::ImportModel("./Assets/Models/gameboard.obj", mesh);
    //meshFilter->mesh = mesh;
    ////A default material
    //std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/TransparentShader.vs", "./StandardShader/TransparentShader.fs"))));
    //material->albedo = Color(1.0f, 1.f, 1.f, 1);
    //material->shininess = 32;
    //material->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/blending_transparent_window.png"));
    //material->renderMode = Material::Transparent;
    //mesh->submeshMaterials[0] = material;
    //std::shared_ptr<MeshRenderer> renderer = object->AddComponent<MeshRenderer>();
    //object->Transform()->scale = Vector3(0.5, 0.5, 0.5);
    //object->Transform()->position = Vector3(0, 0, 0);
    //rootGameObjects.push_back(object);

    std::shared_ptr<GameObject> object2 = std::shared_ptr<GameObject>(new GameObject("Cube"));
    std::shared_ptr<MeshFilter> meshFilter2 = object2->AddComponent<MeshFilter>();
    //mesh->Vertices = vertices_map;
    //mesh->VerticesSize = sizeof(vertices_map);
    std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>(Mesh::Cube());
    //AssetDatabase::ImportModel("./Assets/Models/nanosuit.obj", mesh2);
    meshFilter2->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer2 = object2->AddComponent<MeshRenderer>();
    object2->Transform()->scale = Vector3(1, 1, 1);
    object2->Transform()->position = Vector3(0, -2, 0);
    object2->Transform()->rotation = Quaternion::FromEuler(0, 0, 0);
    rootGameObjects.push_back(object2);

    std::shared_ptr<GameObject> obj3 = std::shared_ptr<GameObject>(new GameObject("Floor"));
    std::shared_ptr<MeshFilter> meshFilter3 = obj3->AddComponent<MeshFilter>();
    meshFilter3->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer3 = obj3->AddComponent<MeshRenderer>();
    obj3->Transform()->scale = Vector3(10, 0.1, 10);
    obj3->Transform()->position = Vector3(0, -3, 0);
    rootGameObjects.push_back(obj3);
    SerializeToFile();
}

void KritiaEngine::SceneManagement::Scene::SerializeToFile() {
    json json;
    json["Type"] = "Scene";
    json["Name"] = name;
    json["Number Of GameObjects"] = rootGameObjects.size();
    int objectIndex = 0;
    for (std::shared_ptr<GameObject> go : rootGameObjects) {
        json["GameObject" + std::to_string(objectIndex)] = go->Serialize();
        objectIndex++;
    }
    std::fstream output = std::fstream();
    std::string fileName = this->name + sceneFilePostfix;
    assert(path != "");
    output.open(path, std::ios::out | std::ios::trunc);
    output << json.dump() << std::endl;
    output.close();
}

void KritiaEngine::SceneManagement::Scene::DeserializeFromPath(const std::string& path) {
    std::ifstream instream(path);
    json json = json::parse(instream);
    assert(json["Type"] == "Scene");
    this->name = json["Name"];
    int numberOfGameObjects = json["Number Of GameObjects"];
    for (int i = 0; i < numberOfGameObjects; i++) {
        std::string str = json["GameObject" + std::to_string(i)];
        // Must separate to avoid parsing error
        nlohmann::ordered_json objectJson = json::parse(str);
        auto gameObject = std::shared_ptr<GameObject>(new GameObject());
        gameObject->name = objectJson["Name"];
        gameObject->DeserializeFromJson(objectJson);
        rootGameObjects.push_back(gameObject);
    }
    instream.close();
}
