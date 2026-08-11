#include <cstdio>

struct ICameraProvider {
    virtual int vf0() = 0;
    virtual int vf1() = 0;
    virtual int vf2() = 0;
    virtual int vf3() = 0;
    virtual int vf4() = 0;
    virtual int GetCurrentMode() = 0;
};

struct CGameCameraManager {
    int                filler0;
    int                filler4;
    int                filler8;
    ICameraProvider*   provider;
    int                mode;
    int HasCameraMode();
};

int CGameCameraManager::HasCameraMode()
{
    return this->provider->GetCurrentMode() != this->mode;
}

struct TestProvider : public ICameraProvider {
    int cur;
    int vf0() { return 0; }
    int vf1() { return 0; }
    int vf2() { return 0; }
    int vf3() { return 0; }
    int vf4() { return 0; }
    int GetCurrentMode() { return cur; }
};

int main()
{
    TestProvider p;
    p.cur = 7;

    CGameCameraManager mgr;
    mgr.provider = &p;

    int fails = 0;

    // provider mode (7) != stored mode (3) -> true
    mgr.mode = 3;
    if (mgr.HasCameraMode() != true) fails++;

    // provider mode (7) == stored mode (7) -> false
    mgr.mode = 7;
    if (mgr.HasCameraMode() != false) fails++;

    p.cur = 42;
    mgr.mode = 42;
    if (mgr.HasCameraMode() != false) fails++;
    mgr.mode = 41;
    if (mgr.HasCameraMode() != true) fails++;

    if (fails == 0) printf("HASCAMERAMODE_OK\n");
    else printf("HASCAMERAMODE_FAIL %d\n", fails);
    return fails == 0 ? 0 : 1;
}