#pragma optimize("s",on)
struct CTarget2 { void Draw(void* a, void* b); };
struct CTarget3 { void Draw(void* a); };

struct CEngineLightingManager {
    char pad[0x18];
    int mode;        // +0x18
    char pad2[0x24 - 0x1c];
    void* obj24;     // +0x24
    void* obj28;     // +0x28
    void Enter(void* d);   // thiscall member, void
};

extern int g_descObj;

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int edx, void** param)
{
    int st;
    self->Enter(&g_descObj);
    st = 0;
    switch (self->mode) {
    case 2:
        ((CTarget2*)&st)->Draw(self->obj24, param);
        break;
    case 3:
        ((CTarget3*)self->obj28)->Draw(*param);
        break;
    }
}