#include "Scene.h"
#include "../Component/MeshFilter.h"
#include "../Component/Renderer/MeshRenderer.h"
#include "MathStructs.h"
#include "../Component/Light.h"
#include "../Editor/AssetDatabase.h"
#include <algorithm>
#include "../Rendering/RenderManager.h"

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
    InitializeRenderQueue();
}

void KritiaEngine::SceneManagement::Scene::InitializeSkybox() {
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/right.jpg")));
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/left.jpg")));
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/top.jpg")));
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/bottom.jpg")));
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/front.jpg")));
    skyboxTextures.push_back(std::shared_ptr<Texture>(new Texture("./Assets/Textures/skybox/back.jpg")));
    RenderManager::LoadCubeMap(skyboxTextures, &skyBoxTextureID);
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
    std::shared_ptr<GameObject> object = std::shared_ptr<GameObject>(new GameObject());
    std::shared_ptr<MeshFilter> meshFilter = object->AddComponent<MeshFilter>();
    //mesh->Vertices = vertices_map;
    //mesh->VerticesSize = sizeof(vertices_map);
    std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(new Mesh());
    AssetDatabase::ImportModel("./Assets/Models/gameboard.obj", mesh);
    meshFilter->mesh = mesh;
    std::shared_ptr<MeshRenderer> renderer = object->AddComponent<MeshRenderer>();
    object->Transform()->Scale = Vector3(0.5, 0.5, 0.5);
    object->Transform()->Position = Vector3(0, 0, 0);
    //A default material
    std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/TransparentShader.vs", "./StandardShader/TransparentShader.fs"))));
    material->albedo = Color(1.0f, 1.f, 1.f, 1);
    material->shininess = 32;
    material->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/blending_transparent_window.png"));
    material->renderMode = Material::Transparent;
    //material->specularMap = std::shared_ptr<Texture>(new Texture("./Assets/Textures/container_specular.png"));
    renderer->materials.push_back(material);
    rootGameObjects.push_back(object);
    
    std::shared_ptr<GameObject> object2 = std::shared_ptr<GameObject>(new GameObject());
    std::shared_ptr<MeshFilter> meshFilter2 = object2->AddComponent<MeshFilter>();
    //mesh->Vertices = vertices_map;
    //mesh->VerticesSize = sizeof(vertices_map);
    std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>(Mesh::Cube());
    //AssetDatabase::ImportModel("./Assets/Models/nanosuit.obj", mesh2);
    meshFilter2->mesh = mesh2;
    std::shared_ptr<MeshRenderer> renderer2 = object2->AddComponent<MeshRenderer>();
    object2->Transform()->Scale = Vector3(0.5, 0.5, 0.5);
    object2->Transform()->Position = Vector3(0, -2, 0);
    //std::shared_ptr<Material> material2 = std::shared_ptr<Material>(new Material("New Material", std::shared_ptr<Shader>(new KritiaEngine::Shader("./StandardShader/TransparentShader.vs", "./StandardShader/TransparentShader.fs"))));
    //material2->albedo = Color(1.0f, 1.f, 1.f, 1);
    //material2->shininess = 32;
    //material2->renderMode = Material::Transparent;
    //material2->mainTexture = std::shared_ptr<Texture>(new Texture("./Assets/Textures/blending_transparent_window.png"));
    //renderer2->materials.push_back(material2);
    rootGameObjects.push_back(object2);
}

void KritiaEngine::SceneManagement::Scene::InitializeRenderQueue() {
    for (std::list<std::shared_ptr<GameObject>>::iterator iter = rootGameObjects.begin(); iter != rootGameObjects.end(); iter++) {
        // check if one object has a transparent material, if yes, put it into transparentObjects
        if ((*iter)->GetComponent<MeshRenderer>() != nullptr) {
            bool opaque = true;
            for (int i = 0; i < (*iter)->GetComponent<MeshRenderer>()->materials.size(); i++) {
                if ((*iter)->GetComponent<MeshRenderer>()->materials[i]->renderMode == Material::Transparent) {
                    opaque = false;
                    transparentObjects.push_back(*iter);
                    break;
                }
            }
            if (opaque) {
                opaqueObjects.push_back(*iter);
            }
        }
    }
}

void KritiaEngine::SceneManagement::Scene::Render(std::shared_ptr<Camera> camera) {
    RenderOpaque(camera);
    RenderSkybox(camera);
    RenderTransparent(camera);
}

void KritiaEngine::SceneManagement::Scene::RenderOpaque(std::shared_ptr<Camera> camera)
{
    for (std::list<std::shared_ptr<GameObject>>::iterator iter = opaqueObjects.begin(); iter != opaqueObjects.end(); iter++) {
        (*iter)->Render(camera);
    }
}

void KritiaEngine::SceneManagement::Scene::RenderTransparent(std::shared_ptr<Camera> camera) {
    std::map<float, std::shared_ptr<GameObject>> sorted = std::map<float, std::shared_ptr<GameObject>>();
    for (std::list<std::shared_ptr<GameObject>>::iterator iter = transparentObjects.begin(); iter != transparentObjects.end(); iter++) {
        float distance = Vector3::Magnitude((*iter)->Transform()->Position - Camera::current->GetPosition());
        sorted[distance] = *iter;
    }
    for (std::map<float, std::shared_ptr<GameObject>>::reverse_iterator iter = sorted.rbegin(); iter != sorted.rend(); iter++) {
        iter->second->Render(camera);
    }
}

void KritiaEngine::SceneManagement::Scene::RenderSkybox(std::shared_ptr<Camera> camera) {
    Matrix4x4 projection = Matrix4x4::Perspective(camera->Zoom, (float)Settings::ScreenWidth / Settings::ScreenHeight, Settings::NearPlaneDistant, Settings::FarPlaneDistant);
    Matrix4x4 view = camera->GetViewMatrix();
    RenderManager::RenderSkybox(skyBoxTextureID, projection, view, skyboxShader);
}

bool KritiaEngine::SceneManagement::Scene::CompareTransparentObjectAndCameraPosition(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2) {
    if (Vector3::Magnitude(obj1->Transform()->Position - Camera::current->GetPosition()) < Vector3::Magnitude(obj2->Transform()->Position - Camera::current->GetPosition())) {
        return true;
    } else {
        return false;
    }
}

void Scene::Update() {
    for (std::list<std::shared_ptr<GameObject>>::iterator iter = rootGameObjects.begin(); iter != rootGameObjects.end(); iter++) {
        if ((*iter)->GetComponent<MonoBehaviour>() != nullptr) {
            (*iter)->GetComponent<MonoBehaviour>()->Update();
        }
    }
}

