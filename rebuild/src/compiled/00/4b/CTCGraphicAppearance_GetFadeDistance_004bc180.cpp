// NGraphicAppearance::CTCGraphicAppearance::GetFadeDistance @ 0x004bc180
// Retail: a0ca863b01 84c0 7407 db0510573701 c3 8b4104 8b4870 d94148 c3
//
// Byte flag global: if set, return (float)g_forcedFadeDistanceInt via fild;
// otherwise return this->m_pAppearanceLink->m_pLodData->m_fadeDistance.
// Modeled __fastcall so 'this' arrives in ecx (byte-identical to __fastcall
// for a this-only accessor). Offsets/globals are relocation-masked in parity.

// --- inlined engine structs (correct offsets only) ---
struct LodData
{
    char _pad0[0x48];
    float m_fadeDistance;   // +0x48  -> fld [ecx+0x48]
};

struct AppearanceLink
{
    char _pad0[0x70];
    LodData *m_pLodData;    // +0x70  -> mov ecx,[eax+0x70]
};

struct CTCGraphicAppearance
{
    char _pad0[0x04];
    AppearanceLink *m_pAppearanceLink;  // +0x04 -> mov eax,[ecx+0x4]
};

// --- relocation-masked globals ---
extern unsigned char g_useForcedFadeDistance; // ds:0x13b86ca  (mov al,ds:imm)
extern int          g_forcedFadeDistanceInt;  // ds:0x1375710  (fild dword ds:imm)

float __fastcall GetFadeDistance(CTCGraphicAppearance *thisptr)
{
    if (g_useForcedFadeDistance)
        return (float)g_forcedFadeDistanceInt;   // fild -> st0
    return thisptr->m_pAppearanceLink->m_pLodData->m_fadeDistance; // fld [ecx+0x48]
}