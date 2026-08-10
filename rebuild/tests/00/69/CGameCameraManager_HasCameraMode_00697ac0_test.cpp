#include <cstdio>

typedef int EMode;

struct CCamera {
    virtual void slot0() {}
    virtual void slot1() {}
    virtual EMode GetCurrentMode() { return mode_val; }
    EMode mode_val;
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

int main()
{
    CCamera cam;
    CGameCameraManager mgr;
    mgr.camera = &cam;

    cam.mode_val = 5;
    if (!mgr.HasCameraMode(5)) { printf("FAIL match\n"); return 1; }
    if (mgr.HasCameraMode(4)) { printf("FAIL nomatch\n"); return 1; }

    cam.mode_val = 0;
    if (!mgr.HasCameraMode(0)) { printf("FAIL zero\n"); return 1; }
    if (mgr.HasCameraMode(1)) { printf("FAIL zero-no\n"); return 1; }

    printf("OK_00697ac0\n");
    return 0;
}