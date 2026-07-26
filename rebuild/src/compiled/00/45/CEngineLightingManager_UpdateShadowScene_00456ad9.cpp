struct LM_Local { void build(void* obj24, void* arg); };
struct LM_Obj28 { void apply(int arg); };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;      /* +0x18 */
    char pad1c[0x24-0x1c];
    void* obj24;   /* +0x24 */
    LM_Obj28* obj28;   /* +0x28 */
};

void __stdcall LM_Guard(const char* name);

#pragma optimize("y", off)
#pragma optimize("s", on)
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, void** arg)
{
    int local;
    LM_Guard((const char*)0x122d70e);
    local = 0;
    switch (self->mode) {
    case 2:
        ((LM_Local*)&local)->build(self->obj24, arg);
        break;
    case 3:
        self->obj28->apply(*(int*)arg);
        break;
    }
}
#pragma optimize("", on)