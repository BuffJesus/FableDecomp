class CGSISheatheHeroWeapons_Target;
class CCreatureActionBase;

class CGSISheatheHeroWeapons_LookupResult
{
public:
    CGSISheatheHeroWeapons_Target* ResolveTarget();
};

class CGSISheatheHeroWeapons_LookupSource
{
public:
    CGSISheatheHeroWeapons_LookupResult* BeginLookup();
};

class CGSISheatheHeroWeapons_Target
{
public:
    bool IsThingCarryingWeapon();
    void PrepareSheathe();
    void UpdateSheathe();
    void FinaliseSheathe();
    void SetCurrentAction(CCreatureActionBase* action);

    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

class CGSISheatheHeroWeapons_ActionStorage
{
public:
    CCreatureActionBase* Construct(
        CGSISheatheHeroWeapons_Target* target,
        CCreatureActionBase* previousAction,
        int priority);
    void Destroy();

private:
    unsigned char m_Bytes[0xB0];
};

struct CGSISheatheHeroWeapons_Interface
{
    unsigned char m_Unused[0x14];
    CGSISheatheHeroWeapons_LookupSource* m_LookupSource;
};

class CGameScriptInterface
{
public:
    virtual void SheatheHeroWeapons() const;
};

void CGameScriptInterface::SheatheHeroWeapons() const
{
    CGSISheatheHeroWeapons_Target* const target =
        reinterpret_cast<
            const CGSISheatheHeroWeapons_Interface*>(
                this)->m_LookupSource
            ->BeginLookup()->ResolveTarget();

    if (target != 0 &&
        (target->m_Flags & 1) == 0 &&
        target->IsThingCarryingWeapon())
    {
        target->PrepareSheathe();
        target->UpdateSheathe();
        target->UpdateSheathe();
        target->FinaliseSheathe();

        CGSISheatheHeroWeapons_ActionStorage actionStorage;
        CCreatureActionBase* const action =
            actionStorage.Construct(
                target,
                0,
                100);
        target->SetCurrentAction(action);
        actionStorage.Destroy();
    }
}
