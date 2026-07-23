#pragma optimize("s",on)
struct CObjA { void Do(int v); };
struct CFreeTgt { void Do(void* a24, void* arg); };

struct CEngineLightingManager {
    char pad0[0x18];
    int mode;
    char pad1c[0x24-0x1c];
    CFreeTgt* obj24;
    CObjA* obj28;
};

extern void __stdcall ProfScope(const char* tag);

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int edxpad, void* arg)
{
    int local;
    ProfScope((const char*)0x122d70e);
    local = 0;
    switch (self->mode) {
    case 2:
        ((CFreeTgt*)&local)->Do(self->obj24, arg);
        break;
    case 3:
        self->obj28->Do(*(int*)arg);
        break;
    }
}