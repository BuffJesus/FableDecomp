// CWorld::IsDisplayingMiniMapOnRegionLoad @ 0x0049e9f0
// Loads a global pointer (ds:0x13b8790), reads a byte flag at +0xf6 on that
// global object; if the flag is set, returns false. Otherwise returns the byte
// field at this+0x12f. this = ecx (__fastcall).

struct CGlobalGui { char pad[0xf6]; bool suppressMiniMap; };
extern CGlobalGui* g_pGlobalGui;  // ds:0x13b8790

struct CWorld {
    char pad[0x12f];
    bool bDisplayMiniMapOnRegionLoad;  // +0x12f
    bool IsDisplayingMiniMapOnRegionLoad();
};

bool CWorld::IsDisplayingMiniMapOnRegionLoad()
{
    if (g_pGlobalGui->suppressMiniMap)
        return false;
    return bDisplayMiniMapOnRegionLoad;
}