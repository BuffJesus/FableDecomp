#pragma optimize("s",on)
#pragma optimize("y",off)
struct CEngineLightingManager {
    char pad00[0x18];
    int mode;        // +0x18
    char pad1c[0x24-0x1c];
    void* obj24;     // +0x24
    void* obj28;     // +0x28
};

struct CShadowLocal {
    int v;
    void Build(void* obj24, int* param);   // default thiscall, 2 stack args
};

struct CShadowTarget {
    void Emit(int arg);                    // default thiscall, 1 stack arg
};

extern void __fastcall LM_Enter(CEngineLightingManager* self, void* edx, void* marker);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* param)
{
    LM_Enter(self, edx, (void*)0x122d70e);
    CShadowLocal local;
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Build(self->obj24, param);
        break;
    case 3:
        ((CShadowTarget*)self->obj28)->Emit(*param);
        break;
    }
}