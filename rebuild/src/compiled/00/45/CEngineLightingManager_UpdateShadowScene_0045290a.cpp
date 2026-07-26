#pragma optimize("s", on)
#pragma optimize("y", off)
extern const char kTag[];

struct CShadowRenderer {
    void Update(void* scene);
};

struct CLocalScene {
    void* p;
    void Fill(void* rendA, void* scene);
};

struct CEngineLightingManager {
    char pad0[0x18];
    int  mode;                 // +0x18
    char pad1[0x24-0x1c];
    void* rendA;               // +0x24
    CShadowRenderer* rendB;    // +0x28

    void Enter(const char* tag);
    void UpdateShadowScene(void** scene);
};

void CEngineLightingManager::UpdateShadowScene(void** scene)
{
    CLocalScene local;
    Enter(kTag);
    local.p = 0;
    switch (mode) {
    case 2:
        local.Fill(rendA, scene);
        break;
    case 3:
        rendB->Update(*scene);
        break;
    }
}