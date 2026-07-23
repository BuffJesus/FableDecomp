struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24-0x1c];
    void* obj24;   // +0x24
    void* obj28;   // +0x28
};

extern void __stdcall EHGuard(int cookie);

struct Obj28 {
    void Handle28(void* arg);
};

struct LocalHelper {
    void Handle24(void* obj24, void* arg);
};

#pragma optimize("ty", off)
#pragma optimize("s", on)
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, void** arg)
{
    int local;
    EHGuard(0x122d70e);
    local = 0;
    switch (self->mode) {
    case 2:
        ((LocalHelper*)&local)->Handle24(self->obj24, arg);
        break;
    case 3:
        ((Obj28*)self->obj28)->Handle28(*arg);
        break;
    }
}
#pragma optimize("y", on)