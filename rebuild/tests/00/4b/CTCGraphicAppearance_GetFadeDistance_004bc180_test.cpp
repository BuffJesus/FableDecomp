// Standalone behaviour test for GetFadeDistance @ 0x004bc180
#include <cstdio>

struct LodData
{
    char _pad0[0x48];
    float m_fadeDistance;   // +0x48
};

struct AppearanceLink
{
    char _pad0[0x70];
    LodData *m_pLodData;    // +0x70
};

struct CTCGraphicAppearance
{
    char _pad0[0x04];
    AppearanceLink *m_pAppearanceLink;  // +0x04
};

// globals (defined here for standalone link)
unsigned char g_useForcedFadeDistance = 0;
int          g_forcedFadeDistanceInt  = 0;

float __fastcall GetFadeDistance(CTCGraphicAppearance *thisptr)
{
    if (g_useForcedFadeDistance)
        return (float)g_forcedFadeDistanceInt;
    return thisptr->m_pAppearanceLink->m_pLodData->m_fadeDistance;
}

int main()
{
    // fixture: object path
    LodData lod;
    lod.m_fadeDistance = 42.5f;
    AppearanceLink link;
    link.m_pLodData = &lod;
    CTCGraphicAppearance obj;
    obj.m_pAppearanceLink = &link;

    // case 1: flag clear -> follow object chain
    g_useForcedFadeDistance = 0;
    float r1 = GetFadeDistance(&obj);

    // case 2: flag set -> fild of int global
    g_useForcedFadeDistance = 1;
    g_forcedFadeDistanceInt = 100;
    float r2 = GetFadeDistance(&obj);

    bool ok = (r1 == 42.5f) && (r2 == 100.0f);
    if (ok)
        printf("CTCGRAPHICAPPEARANCE_004bc180_TEST PASS\n");
    else
        printf("CTCGRAPHICAPPEARANCE_004bc180_TEST FAIL r1=%f r2=%f\n", r1, r2);
    return ok ? 0 : 1;
}