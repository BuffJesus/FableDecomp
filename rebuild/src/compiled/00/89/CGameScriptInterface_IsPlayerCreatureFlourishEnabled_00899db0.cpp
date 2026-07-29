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
    unsigned char m_Pad00[0x24];
    unsigned int  m_Flags24;       // 0x24
    unsigned char m_Pad28[0x1c];
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
extern "C" bool __fastcall Sub_IsMeleeFlourishValid(void* iface);

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x30;

bool __fastcall CGameScriptInterface_IsPlayerCreatureFlourishEnabled(
    const CGameScriptInterface* self)
{
    void* a = Sub_ResolveA(self->m_Field14);
    CThingOverlay* pThing = (CThingOverlay*)Sub_ResolveB(a);
    if (pThing != 0 &&
        (pThing->m_Flags91 & 0x01) == 0 &&
        (pThing->m_Flags24 & 0x10000) != 0)
    {
        CVectorMap* pMap = &pThing->m_InterfaceMap44;
        ETCInterfaceType key = kTargetType;
        CKeyPair* pEntry = pMap->LowerBound(&key);
        CKeyPair* pEnd = pMap->m_pEnd;
        if (pEntry == pEnd || (int)pEntry->m_Key > 0x30)
            pEntry = pEnd;

        return Sub_IsMeleeFlourishValid(pEntry->m_Value);
    }
    return false;
}
