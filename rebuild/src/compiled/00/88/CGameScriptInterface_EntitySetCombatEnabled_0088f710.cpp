class CCharString;
class CGameScriptInterface;
class CScriptThing;

class CCharString
{
public:
    CCharString(const char*, int);
    ~CCharString();

private:
    void* storage_;
};

class CGameScriptInterface
{
public:
    void EntitySetCombatEnabled(const CScriptThing&, bool) const;
};

typedef void (CGameScriptInterface::*DispatchFn)(
    const CScriptThing&,
    const CCharString&,
    bool) const;

struct VTableOverlay
{
    unsigned char pad[0x938];
    DispatchFn dispatch;
};

struct InterfaceOverlay
{
    const VTableOverlay* vtable;
};

void CGameScriptInterface::EntitySetCombatEnabled(
    const CScriptThing& entity,
    bool enabled) const
{
    const InterfaceOverlay* p = (const InterfaceOverlay*)this;
    {
        CCharString state("SG_SEEK_ENEMY", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY2", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
    {
        CCharString state("SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE", -1);
        (this->*(p->vtable->dispatch))(entity, state, enabled);
    }
}