class CCreatureActionBase;
class CScriptThing;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    bool SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    bool EntityCastForcePush(const CScriptThing& thing, bool forcePush) const;
};

namespace
{
struct CScriptThing_VTable_Overlay
{
    unsigned char m_Pad00[0x2C];
    CThingCreatureBase* (__fastcall* GetThing)(const CScriptThing* pScriptThing);
    unsigned char m_Pad30[0xFC];
    bool (__fastcall* IsValidThing)(const CScriptThing* pScriptThing);
};

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable;
};

struct CThingCreatureBase_Overlay
{
    unsigned char m_Pad00[0x6C];
    unsigned char m_Flags6C;
};

struct CCreatureAction_SheatheWeapons_Overlay
{
    unsigned char m_Pad00[0x114];
    unsigned char m_Field110;
    unsigned char m_Pad115[0x03];
    void Construct(CThingCreatureBase* pCreature);
    void Destruct();
};
}

bool CGameScriptInterface::EntityCastForcePush(
    const CScriptThing& thing,
    bool forcePush) const
{
    const CScriptThing_Overlay* const thingOverlay =
        reinterpret_cast<const CScriptThing_Overlay*>(&thing);

    if (thingOverlay->m_pVTable->IsValidThing(&thing))
    {
        CThingCreatureBase* const pCreature = thingOverlay->m_pVTable->GetThing(&thing);
        if ((reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature)->m_Flags6C & 0x08) != 0)
        {
            CCreatureAction_SheatheWeapons_Overlay action;
            action.Construct(pCreature);

            if (forcePush)
            {
                action.m_Field110 = 1;
            }

            const bool result =
                pCreature->SetCurrentAction(reinterpret_cast<CCreatureActionBase*>(&action));

            action.Destruct();
            return result;
        }
    }

    return false;
}