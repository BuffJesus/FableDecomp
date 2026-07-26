#pragma optimize("s",on)
struct CEngineLightingManager {
    char pad[0x18];
    int state;      /* +0x18 */
    char pad2[0x24-0x1c];
    int field24;    /* +0x24 */
    void* field28;  /* +0x28 */
};

struct CShadowSceneA { void Set(int v); };
struct CShadowSceneB { void Put(int v); };

extern void __stdcall PrepShadow(int tag);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, int* p)
{
    PrepShadow(0x122d70e);
    switch (self->state) {
    case 2:
        ((CShadowSceneB*)p)->Put(self->field24);
        break;
    case 3:
        ((CShadowSceneA*)self->field28)->Set(*p);
        break;
    }
}