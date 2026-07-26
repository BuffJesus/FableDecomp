extern unsigned char g_CheapHeadLooking;

struct CGameScriptInterface;

bool __fastcall CGameScriptInterface_GetCheapHeadLooking(const CGameScriptInterface* self)
{
    (void)self;
    bool r;
    *(unsigned char*)&r = g_CheapHeadLooking;
    return r;
}