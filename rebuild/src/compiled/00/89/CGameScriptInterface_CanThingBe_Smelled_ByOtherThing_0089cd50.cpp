class CScriptThing;

class CGameScriptInterface
{
public:
    virtual bool CanThingBe_Smelled_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing) const;

private:
    bool CanThingBe_Sensed_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing,
        long senseType) const;
};

bool CGameScriptInterface::CanThingBe_Smelled_ByOtherThing(
    const CScriptThing& thing,
    const CScriptThing& otherThing) const
{
    return CanThingBe_Sensed_ByOtherThing(
        thing,
        otherThing,
        3);
}
