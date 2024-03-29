#include "Scene.h"
#include "../Component/MeshFilter.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "MathStructs.h"
#include "../Component/Light.h"
#include "../Editor/AssetDatabase.h"
#include "../Rendering/RenderingProvider.h"
#include "../Editor/EditorApplication.h"
#include "Manager/ResourceManager.h"
#include <fstream>

using namespace KritiaEngine::SceneManagement;
using namespace KritiaEngine::Lighting;
using namespace KritiaEngine;
using namespace KritiaEngine::Editor;
using json = nlohmann::ordered_json;



std::list<GameObject*>& KritiaEngine::SceneManagement::Scene::GetRootGameObjects()
{
    return rootGameObjects;
}

KritiaEngine::SceneManagement::Scene::Scene(const std::string &name, const std::string& path)
{
    this->name = name;
    this->path = path;
}

KritiaEngine::SceneManagement::Scene::~Scene() {
    for (GameObject* go : rootGameObjects) {
        delete go;
    }
    rootGameObjects.clear();
}

void KritiaEngine::SceneManagement::Scene::Initialize() {
#ifdef  Generate
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
        InitializeGameObjects();
    }
#else
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
        //InitializeGameObjects();
    }
    //InitializeGameObjects();
#endif //  Generate
}

void KritiaEngine::SceneManagement::Scene::InitializeCamera()
{
    GameObject* mainCamera = new GameObject("Main Camera");
    currentCamera = mainCamera;
    currentCamera->AddComponent<Camera>();
    Camera::current = currentCamera->GetComponent<Camera>();
}

void Scene::InitializeLighting() {
    GameObject* mainLightSource = new GameObject("Main Light Source");
    this->mainLightSource = mainLightSource;
    std::shared_ptr<Light> light = this->mainLightSource->AddComponent<Light>();
    light->color = Color(1, 1, 1, 1);
    light->Transform()->forward = Vector3::Normalize(Vector3(-1, 0, 1));
    LightingSystem::SetMainLightSource(this->mainLightSource->GetComponent<Light>().get());
}

void Scene::InitializeGameObjects() {
    GameObject* object2 = new GameObject("Cube");
    std::shared_ptr<MeshFilter> meshFilter2 = object2->AddComponent<MeshFilter>();
    //mesh->Vertices = vertices_map;
    //mesh->VerticesSize = sizeof(vertices_map);
    std::shared_ptr<Mesh> mesh2 = Manager::ResourceManager::GetMeshPrimitive(Manager::ResourceManager::MeshPrimitive::Cube);
    //AssetDatabase::ImportModel("./Assets/Models/nanosuit.obj", mesh2);
    meshFilter2->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer2 = object2->AddComponent<MeshRenderer>();
    object2->Transform()->scale = Vector3(1, 1, 1);
    object2->Transform()->position = Vector3(0, 0, 0);
    object2->Transform()->rotation = Quaternion::FromEuler(0, 0, 0);

    GameObject* obj3 = new GameObject("Floor");
    std::shared_ptr<MeshFilter> meshFilter3 = obj3->AddComponent<MeshFilter>();
    meshFilter3->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer3 = obj3->AddComponent<MeshRenderer>();
    obj3->Transform()->scale = Vector3(10, 0.1, 10);
    obj3->Transform()->position = Vector3(0, -2, 0);
}

void KritiaEngine::SceneManagement::Scene::SerializeToFile() {
    json json;
    json["Type"] = "Scene";
    json["Name"] = name;
    json["Number Of GameObjects"] = rootGameObjects.size();
    int objectIndex = 0;
    for (GameObject* go : rootGameObjects) {
        json["GameObject" + std::to_string(objectIndex)] = go->SerializeToJson();
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
        GameObject* gameObject = GameObject::DeserializeFromJson(objectJson);
    }
    instream.close();
}
