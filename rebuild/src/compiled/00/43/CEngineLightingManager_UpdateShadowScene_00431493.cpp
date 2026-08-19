#pragma optimize("s",on)
extern "C" void __stdcall Profile_Enter(int marker);

extern void __fastcall LM_ShadowApply24(void* self24, void* arg);
extern void __fastcall LM_ShadowApply28(void* self28, void* arg);

struct CEngineLightingManager {
    char pad[0x18];
    int mode;                  /* +0x18 */
    char pad2[0x24 - 0x1c];
    void* p24;                 /* +0x24 */
    void* p28;                 /* +0x28 */
};

void __fastcall CEngineLightingManager_UpdateShadowScene(
    CEngineLightingManager* self, void* /*edx*/, void* arg)
{
    Profile_Enter(0x122d70e);
    switch (self->mode) {
    case 2:
        LM_ShadowApply24(self->p24, arg);
        break;
    case 3:
        LM_ShadowApply28(self->p28, arg);
        break;
    }
}