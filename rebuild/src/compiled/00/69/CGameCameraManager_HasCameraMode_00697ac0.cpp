typedef int EMode;

struct CCamera {
    virtual void slot0();
    virtual void slot1();
    virtual EMode GetCurrentMode();
};

struct CGameCameraManager {
    char pad[0x98];
    CCamera* camera;
    bool HasCameraMode(EMode mode);
};

bool CGameCameraManager::HasCameraMode(EMode mode)
{
    return this->camera->GetCurrentMode() == mode;
}