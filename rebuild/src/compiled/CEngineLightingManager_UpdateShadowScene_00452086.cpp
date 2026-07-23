#pragma optimize("s",on)

extern "C" void __stdcall Profile_Enter(int marker);

struct ShadowSceneLocal {
    int v;
    void Build(void* a, void* b);
};

struct CShadowSceneTarget {
    void Apply(void* a);
};

struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* p24;                 // +0x24
    CShadowSceneTarget* p28;   // +0x28
    void UpdateShadowScene(void* arg);
};

void CEngineLightingManager::UpdateShadowScene(void* arg)
{
    Profile_Enter(0x122d70e);
    ShadowSceneLocal loc;
    loc.v = 0;
    switch (this->mode) {
    case 2:
        loc.Build(this->p24, arg);
        break;
    case 3:
        this->p28->Apply(*(void**)arg);
        break;
    }
}