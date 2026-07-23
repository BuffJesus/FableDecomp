#pragma optimize("s",on)
#pragma optimize("y",off)
extern void __stdcall Setup(const void* p);

struct CObjA { void DoA(void* arg); };
struct TLocal { int v; void DoB(void* a, void** b); };

struct CEngineLightingManager {
    char pad[0x18];
    int field_18;      // +0x18
    char pad2[0x24-0x1c];
    CObjA* field_24;   // +0x24
    CObjA* field_28;   // +0x28
    void UpdateShadowScene(void** pArg);
};

void CEngineLightingManager::UpdateShadowScene(void** pArg)
{
    TLocal local;
    Setup((const void*)0x122d70e);
    local.v = 0;
    switch (field_18) {
    case 2:
        local.DoB(field_24, pArg);
        break;
    case 3:
        field_28->DoA(*pArg);
        break;
    }
}