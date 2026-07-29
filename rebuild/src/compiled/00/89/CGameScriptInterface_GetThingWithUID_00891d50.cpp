class CStopWatch;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
    virtual ~CBase();
};
}

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing()
        : m_Field04(0),
          m_Field08(0)
    {
    }

    virtual ~CScriptThing();

private:
    unsigned long m_Field04;
    unsigned long m_Field08;
};

class CStopWatch
{
public:
    void* GetTicks(unsigned __int64 uid) const;
};

class CGSIGetThingWithUID_Dispatch
{
public:
    virtual void Reserved00();
    virtual void Reserved04();
    virtual void Reserved08();
    virtual CStopWatch* GetStopWatch();
};

struct CGSIGetThingWithUID_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIGetThingWithUID_Dispatch* m_pDispatch;
};

class CGSIGetThingWithUID_Thing
{
public:
    CScriptThing WrapForScript() const;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing GetThingWithUID(unsigned __int64 uid) const;
};

CScriptThing CGameScriptInterface::GetThingWithUID(
    unsigned __int64 uid) const
{
    const CGSIGetThingWithUID_InterfaceOverlay* const pThis =
        reinterpret_cast<const CGSIGetThingWithUID_InterfaceOverlay*>(this);

    void* const pThing =
        pThis->m_pDispatch
            ->GetStopWatch()
            ->GetTicks(uid);

    if (pThing != 0)
    {
        return reinterpret_cast<const CGSIGetThingWithUID_Thing*>(pThing)
            ->WrapForScript();
    }

    return CScriptThing();
}
