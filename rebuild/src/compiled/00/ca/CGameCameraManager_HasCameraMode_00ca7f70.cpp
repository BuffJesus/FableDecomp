// CGameCameraManager::HasCameraMode  @ 00ca7f70
// __fastcall bool(CGameCameraManager*)   (retail ret is 0xC3 -> no stack param)

struct ICameraProvider {
    virtual int vf0() = 0;
    virtual int vf1() = 0;
    virtual int vf2() = 0;
    virtual int vf3() = 0;
    virtual int vf4() = 0;
    virtual int GetCurrentMode() = 0;  // slot 5 -> [vtable+0x14]
};

struct CGameCameraManager {
    int                filler0;      // +0x0
    int                filler4;      // +0x4
    int                filler8;      // +0x8
    ICameraProvider*   provider;     // +0xC
    int                mode;         // +0x10
    int HasCameraMode();
};

int CGameCameraManager::HasCameraMode()
{
    return this->provider->GetCurrentMode() != this->mode;
}