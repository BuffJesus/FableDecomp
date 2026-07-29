enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CKeyPair
{
    ETCInterfaceType m_Key;   // 0x00
    void*            m_Value; // 0x04
};

struct CVectorMap
{
    CKeyPair* m_pBegin; // 0x00
    CKeyPair* m_pEnd;   // 0x04

    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x2c];
    unsigned int  m_Flags2C;       // 0x2c
    unsigned char m_Pad30[0x14];
    CVectorMap    m_InterfaceMap44; // 0x44
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;        // 0x91
};

struct CGameScriptInterface
{
    unsigned char m_Pad00[0x14];
    void*         m_Field14; // 0x14
};

extern "C" void* __fastcall Sub_ResolveA(void* self);
extern "C" void* __fastcall Sub_ResolveB(void* obj);
extern "C" long __fastcall Sub_GetExperiencePickUpMultiplier(void* iface);

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x68;

long __fastcall CGameScriptInterface_GetPlayerCreatureCombatMultiplier(
    const CGameScriptInterface* self)
{
    void* a = Sub_ResolveA(self->m_Field14);
    CThingOverlay* pThing = (CThingOverlay*)Sub_ResolveB(a);
    if (pThing != 0 &&
        (pThing->m_Flags91 & 0x01) == 0 &&
        (pThing->m_Flags2C & 0x100) != 0)
    {
        CVectorMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CKeyPair* pEntry = pMap->LowerBound(&key);
        CKeyPair* pEnd = pMap->m_pEnd;
        if (pEntry == pEnd || (int)pEntry->m_Key > 0x68)
            pEntry = pEnd;

        return Sub_GetExperiencePickUpMultiplier(pEntry->m_Value);
    }
    return 1;
}
