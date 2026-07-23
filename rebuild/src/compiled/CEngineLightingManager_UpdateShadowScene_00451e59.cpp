#pragma optimize("s",on)
// CEngineLightingManager::UpdateShadowScene @ 00451e59  __fastcall void (this, void* arg)

struct Local4 { int v; void Fill(void* obj24, void* arg); };
struct Obj28 { void Draw(int v); };

struct CEngineLightingManager {
    char pad_18[0x18];
    int mode;               // +0x18  scene/shadow mode selector
    char pad_1c[0x24-0x1c];
    void* obj24;            // +0x24
    Obj28* obj28;           // +0x28
};

extern "C" void __stdcall ProfileScope(unsigned int hash);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, void* arg)
{
    Local4 local;
    ProfileScope(0x122d70e);
    local.v = 0;
    switch (self->mode) {
    case 2:
        local.Fill(self->obj24, arg);
        break;
    case 3:
        self->obj28->Draw(*(int*)arg);
        break;
    }
}