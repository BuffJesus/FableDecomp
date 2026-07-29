class CBaseObject;
class CScriptThing;
class CThing;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
};
}

extern const unsigned char
    CGSIGetMostRecentValidUsedTarget_EmptyVTable;

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing()
        : m_VTable(
              &CGSIGetMostRecentValidUsedTarget_EmptyVTable),
          m_Field04(0),
          m_Field08(0)
    {
    }

private:
    const void* volatile m_VTable;
    volatile unsigned long m_Field04;
    volatile unsigned long m_Field08;
};

class CBaseObject
{
public:
    CScriptThing BuildScriptThing();
};

class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem();
};

class CVillageTaskCarryCrate
{
public:
    void AssignCreatureToTask(CThing* thing);

private:
    unsigned char m_Unused[0xA8];

public:
    CBaseIntelligentPointer m_MostRecentTarget;
};

struct CGSIGetMostRecentValidUsedTarget_BaseObject
{
    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing
        GetMostRecentValidUsedTarget() const;
};

extern CVillageTaskCarryCrate*
    CGSIGetMostRecentValidUsedTarget_Manager;

CScriptThing
CGameScriptInterface::GetMostRecentValidUsedTarget() const
{
    CBaseObject* const item =
        CGSIGetMostRecentValidUsedTarget_Manager
            ->m_MostRecentTarget.GetPItem();
    if (item != 0 &&
        (reinterpret_cast<
            const CGSIGetMostRecentValidUsedTarget_BaseObject*>(
                item)->m_Flags & 1) == 0)
    {
        CGSIGetMostRecentValidUsedTarget_Manager
            ->AssignCreatureToTask(0);
        return item->BuildScriptThing();
    }

    return CScriptThing();
}
