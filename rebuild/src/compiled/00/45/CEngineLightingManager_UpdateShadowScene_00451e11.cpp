// source_cpp
struct LocalObj {
    char pad[16];
    LocalObj();
    void apply24(void* fld, int arg);
    void apply28(void* fld, int arg);
};

extern void __stdcall ProfileEnter(int tag);

struct CEngineLightingManager {
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* p24;     // +0x24
    void* p28;     // +0x28
};

#pragma optimize("s", on)
#pragma optimize("y", off)
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, int /*edx*/, int arg)
{
    ProfileEnter(0x122d70e);
    LocalObj local;
    switch (self->mode) {
    case 2:
        local.apply24(self->p24, arg);
        break;
    case 3:
        local.apply28(self->p28, arg);
        break;
    }
}