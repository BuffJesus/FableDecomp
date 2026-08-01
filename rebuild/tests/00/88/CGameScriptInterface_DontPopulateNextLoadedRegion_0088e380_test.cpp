#include <cstdio>

struct COwner
{
    unsigned char m_Pad00[0xD8];
    unsigned char m_bDontPopulateNextLoadedRegion; // 0xD8
};

struct CGameScriptInterface
{
    void* m_Pad00;   // 0x0
    COwner* m_pOwner; // 0x4
};

void __fastcall CGameScriptInterface_DontPopulateNextLoadedRegion(CGameScriptInterface* self)
{
    self->m_pOwner->m_bDontPopulateNextLoadedRegion = 1;
}

int main()
{
    COwner owner;
    owner.m_bDontPopulateNextLoadedRegion = 0;
    CGameScriptInterface iface;
    iface.m_Pad00 = 0;
    iface.m_pOwner = &owner;
    CGameScriptInterface_DontPopulateNextLoadedRegion(&iface);
    if (owner.m_bDontPopulateNextLoadedRegion == 1)
        printf("DONTPOP_OK\n");
    else
        printf("DONTPOP_FAIL\n");
    return 0;
}