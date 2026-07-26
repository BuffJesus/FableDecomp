#include <cstdio>

struct CCharString { int dummy; };
struct CRegions;

int g_count = 0;

struct CMid { void** vt; };
struct CGameScriptInterface { void* vt; CMid* mid; };

typedef CRegions* (CMid::*PSlot0)();
typedef CRegions* (CMid::*PSlot1)(int);

char g_regions[0x18 + sizeof(CCharString)];
CCharString g_default;

extern "C" int __fastcall CRegions_GetCount(CRegions* r) { return g_count; }
extern "C" void* __fastcall CRegions_GetEntry(CRegions* r) { return (void*)g_regions; }

// Free-function stand-ins for the vtable slot (native thiscall via /Gd default is
// fine here since we only test behaviour, not bytes).
CRegions* slot_noarg(void* self) { return (CRegions*)g_regions; }
CRegions* slot_arg(void* self, int n) { return (CRegions*)g_regions; }

const CCharString& __fastcall CGameScriptInterface_GetRegionName(CGameScriptInterface* self)
{
    CMid* m = self->mid;
    // Behavioural model (call the collection accessor, count, maybe fetch entry).
    CRegions* col = (CRegions*)slot_noarg(m);
    int n = CRegions_GetCount(col);
    if (n > 0) {
        CMid* m2 = self->mid;
        CRegions* e = (CRegions*)slot_arg(m2, n);
        return *(const CCharString*)((char*)CRegions_GetEntry(e) + 0x18);
    }
    return *(const CCharString*)&g_default;
}

int main() {
    void* vtbl[16];
    CMid midObj; midObj.vt = vtbl;
    CGameScriptInterface gsi; gsi.vt = 0; gsi.mid = &midObj;

    g_count = 0;
    const CCharString& r1 = CGameScriptInterface_GetRegionName(&gsi);
    if (&r1 != &g_default) { std::printf("FAIL default path\n"); return 1; }

    g_count = 3;
    const CCharString& r2 = CGameScriptInterface_GetRegionName(&gsi);
    if ((char*)&r2 != g_regions + 0x18) { std::printf("FAIL entry path\n"); return 1; }

    std::printf("CGameScriptInterface_0088e340_TEST PASS\n");
    return 0;
}