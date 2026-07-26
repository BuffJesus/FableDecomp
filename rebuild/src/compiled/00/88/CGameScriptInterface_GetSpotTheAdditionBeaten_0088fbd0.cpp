extern bool g_SpotTheAdditionBeaten;

struct CGameScriptInterface {};

bool __fastcall CGameScriptInterface_GetSpotTheAdditionBeaten(const CGameScriptInterface* self)
{
    (void)self;
    return g_SpotTheAdditionBeaten;
}