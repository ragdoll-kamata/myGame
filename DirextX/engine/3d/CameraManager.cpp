#include "CameraManager.h"
#include <cassert>

CameraManager* CameraManager::instance = nullptr;
void CameraManager::Update()
{
    activeCamera->Update();
}
void CameraManager::CreateNewCamera(std::string name)
{
    std::unique_ptr<Camera> camera = std::make_unique<Camera>();

    cameras.insert(std::make_pair(name, std::move(camera)));
}
bool CameraManager::IsExistCamera(std::string name)
{
    if (cameras.contains(name)) {
        return true;
    }
    return false;
}
void CameraManager::SetActiveCamera(std::string name)
{
    if (cameras.contains(name)) {
        activeCamera = cameras[name].get();
    } else {
        assert(false);
    }
}
Camera* CameraManager::GetCamera(std::string name)
{
    if (cameras.contains(name)) {
        return cameras[name].get();
    }
    assert(false);
    return nullptr;
}
CameraManager* CameraManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new CameraManager;
        instance->Initialize();
    }
    return instance;
}

void CameraManager::Initialize()
{
    CreateNewCamera("default");
    SetActiveCamera("default");
}
