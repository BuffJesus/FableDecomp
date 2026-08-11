#include <cstdio>

enum EMode { EM_0 = 0, EM_1 = 1 };

struct ICamProvider {
    virtual int vf0() = 0;
    virtual int vf1() = 0;
    virtual int vf2() = 0;
    virtual int vf3() = 0;
    virtual int GetMode() = 0;
};

struct CGameCameraManager {
    int pad0;
    int pad4;
    ICamProvider* prov;
    int curMode;
};

int __fastcall CGameCameraManager_HasCameraMode(CGameCameraManager* self, EMode mode)
{
    return self->prov->GetMode() != self->curMode;
}

struct FakeProvider : ICamProvider {
    int mode;
    int vf0() { return 0; }
    int vf1() { return 0; }
    int vf2() { return 0; }
    int vf3() { return 0; }
    int GetMode() { return mode; }
};

int main()
{
    FakeProvider fp;
    fp.mode = 5;

    CGameCameraManager m;
    m.prov = &fp;
    m.curMode = 5;

    int r1 = CGameCameraManager_HasCameraMode(&m, EM_0);   // 5 != 5 -> 0
    fp.mode = 7;
    int r2 = CGameCameraManager_HasCameraMode(&m, EM_0);   // 7 != 5 -> 1

    if (r1 == 0 && r2 == 1) {
        printf("HASCAMERAMODE_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}