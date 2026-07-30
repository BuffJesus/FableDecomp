#include <stdio.h>

// ---- Globals mirroring source_cpp ----
struct CGlobalGui { char pad[0xf6]; unsigned char suppressMiniMap; };
static CGlobalGui g_globalGuiInst;
CGlobalGui* g_pGlobalGui = &g_globalGuiInst;

struct CWorld {
    char pad[0x12f];
    unsigned char bDisplayMiniMapOnRegionLoad;  // +0x12f
};

// Function under test, __fastcall so this=ecx, no stack args, bool in al.
static bool __fastcall IsDisplayingMiniMapOnRegionLoad(CWorld* self)
{
    if (g_pGlobalGui->suppressMiniMap)
        return false;
    return self->bDisplayMiniMapOnRegionLoad != 0;
}

int main()
{
    CWorld w;

    // Case 1: global suppress flag SET -> always returns false.
    g_pGlobalGui->suppressMiniMap = 1;
    w.bDisplayMiniMapOnRegionLoad = 1;
    bool r1 = IsDisplayingMiniMapOnRegionLoad(&w);

    // Case 2: suppress clear, field true -> returns true.
    g_pGlobalGui->suppressMiniMap = 0;
    w.bDisplayMiniMapOnRegionLoad = 1;
    bool r2 = IsDisplayingMiniMapOnRegionLoad(&w);

    // Case 3: suppress clear, field false -> returns false.
    w.bDisplayMiniMapOnRegionLoad = 0;
    bool r3 = IsDisplayingMiniMapOnRegionLoad(&w);

    if (r1 == false && r2 == true && r3 == false) {
        printf("OK_0x0049e9f0 suppress-forces-false field-passthrough\n");
        return 0;
    }
    printf("FAIL r1=%d r2=%d r3=%d\n", (int)r1, (int)r2, (int)r3);
    return 1;
}