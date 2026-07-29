enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CScriptThing;
struct CThingOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CThingOverlay* (__fastcall* m_Resolve)(CScriptThing* self); // 0x2c
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self); // 0x12c
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

class CGSIPickLockableDoor
{
public:
    bool IsLockPickable();
};

struct CKeyPair
{
    ETCInterfaceType      m_Key;   // 0x00
    CGSIPickLockableDoor* m_Value; // 0x04
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
};

struct CGameScriptInterface
{
};

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x26;

bool __fastcall CGameScriptInterface_IsEntityPickLockable(
    const CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing)
{
    (void)self;
    (void)edxUnused;
    if (pScriptThing->m_pVTable->m_IsAvailable(pScriptThing))
    {
        CThingOverlay* pThing =
            pScriptThing->m_pVTable->m_Resolve(pScriptThing);
        if ((pThing->m_Flags24 & 0x40) != 0)
        {
            CVectorMap* pMap = &pThing->m_InterfaceMap44;
            ETCInterfaceType key = kTargetType;
            CKeyPair* pEntry = pMap->LowerBound(&key);
            CKeyPair* pEnd = pMap->m_pEnd;
            if (pEntry == pEnd || (int)pEntry->m_Key > 0x26)
                pEntry = pEnd;

            if (pEntry->m_Value->IsLockPickable())
                return true;
        }
    }
    return false;
}
