class CScriptThing;

class CGameScriptInterface
{
public:
    void ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing, bool isAlly) const;
    void EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const;
};

void CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing& entity, const CScriptThing& otherThing) const
{
    ImpSetEntityAsAllyOfThing(entity, otherThing, true);
}