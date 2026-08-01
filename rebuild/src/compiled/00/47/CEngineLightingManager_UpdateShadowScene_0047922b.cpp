struct CShadowTarget28 {
    void Do(int v);
};
struct CLocalScene {
    int state;
    void Run(void* obj24, int* arg);
};

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;              // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;           // +0x24
    CShadowTarget28* obj28;// +0x28
};

#pragma optimize("s", on)
#pragma optimize("y", off)

extern void __stdcall EnterGuard(void* p);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int /*edx*/, int* arg)
{
    CLocalScene local;
    EnterGuard((void*)0x122d70e);
    local.state = 0;
    switch (self->mode) {
    case 2:
        local.Run(self->obj24, arg);
        break;
    case 3:
        self->obj28->Do(*arg);
        break;
    }
}