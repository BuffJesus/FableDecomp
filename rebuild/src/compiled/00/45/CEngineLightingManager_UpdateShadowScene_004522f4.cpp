#pragma optimize("s",on)
// CEngineLightingManager::UpdateShadowScene @ 004522f4
struct CShadowArg;

struct CShadowSink {
    void Consume(CShadowArg* a); // thiscall
};

struct CEngineLightingManager {
    char pad0[0x18];
    int  mode;      // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;    // +0x24
    void* obj28;    // +0x28 (CShadowSink*)
};

struct CLocalHolder {
    int v; // [ebp-4]
    void Handle(void* arg, void* obj); // thiscall (push arg then push obj)
};

extern "C" void __stdcall EnterShadowScope(void* tag);
extern int g_shadowTag;

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, CShadowArg** ppArg)
{
    (void)edx;
    EnterShadowScope((void*)&g_shadowTag);
    CLocalHolder local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Handle(self->obj24, ppArg);
        break;
    case 3:
        ((CShadowSink*)self->obj28)->Consume(*ppArg);
        break;
    }
}