#pragma optimize("s",on)

extern "C" void __stdcall LogShadowScene(int tag);

struct Local4 { int v; void Consume(void* obj24, int* param); };
struct Callee28 { void Fire(int a); };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;      // +0x18
    char pad1c[0x24 - 0x1c];
    void* obj24;   // +0x24
    Callee28* obj28;   // +0x28
};

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* param)
{
    Local4 local;
    LogShadowScene(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Consume(self->obj24, param);
        break;
    case 3:
        self->obj28->Fire(*param);
        break;
    }
}