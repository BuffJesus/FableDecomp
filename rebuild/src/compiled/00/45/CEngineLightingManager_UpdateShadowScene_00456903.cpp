#pragma optimize("s", on)
#pragma optimize("y", off)
struct Vec12 { int a, b, c; };

struct CEngineLightingManager {
    char pad[0x18];
    int mode;        // +0x18
    char pad2[0x24-0x18-4];
    void* obj24;     // +0x24
    void* obj28;     // +0x28
};

struct PBox { Vec12* p; };
extern void __stdcall Prof_Enter(const char* tag);
extern void __fastcall LM_Handle3(void* self28, PBox p); // __fastcall(ecx=obj28, stack arg=param ptr)
extern Vec12 __fastcall LM_Handle2(void* self24);          // __fastcall(ecx=obj24), returns Vec12 by value

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, Vec12* param)
{
    Prof_Enter((const char*)0x122d70e);
    switch (self->mode) {
    case 2:
        *param = LM_Handle2(self->obj24);
        break;
    case 3: {
        PBox b; b.p = param;
        LM_Handle3(self->obj28, b);
        break;
    }
    }
}