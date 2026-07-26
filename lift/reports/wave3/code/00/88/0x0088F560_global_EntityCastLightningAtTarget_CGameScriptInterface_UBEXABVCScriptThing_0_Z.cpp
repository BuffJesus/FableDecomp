class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall EntityCastLightningAtTarget(
        const CScriptThing& sourceThing,
        const CScriptThing& targetThing) const;
};

void __thiscall CGameScriptInterface::EntityCastLightningAtTarget(
    const CScriptThing& sourceThing,
    const CScriptThing& targetThing) const
{
    (void)sourceThing;
    (void)targetThing;
}