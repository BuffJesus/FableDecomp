class CCreatureActionBase;
class CThingCreatureBase;

class CBaseIntelligentPointer
{
public:
    CThingCreatureBase* GetPItem() const;
};

struct HeroHandle
{
    unsigned char unused00[0x34];
    CBaseIntelligentPointer item;
};

class HeroResolver
{
public:
    HeroHandle* ResolveHero();
};

class CCreatureActionBase
{
public:
    virtual ~CCreatureActionBase();
};

class CThingCreatureBase
{
public:
    virtual void Slot000(); virtual void Slot004(); virtual void Slot008(); virtual void Slot00C();
    virtual void Slot010(); virtual void Slot014(); virtual void Slot018(); virtual void Slot01C();
    virtual void Slot020(); virtual void Slot024(); virtual void Slot028(); virtual void Slot02C();
    virtual void Slot030(); virtual void Slot034(); virtual void Slot038(); virtual void Slot03C();
    virtual void Slot040(); virtual void Slot044(); virtual void Slot048(); virtual void Slot04C();
    virtual void Slot050(); virtual void Slot054(); virtual void Slot058(); virtual void Slot05C();
    virtual void Slot060(); virtual void Slot064(); virtual void Slot068(); virtual void Slot06C();
    virtual void Slot070(); virtual void Slot074(); virtual void Slot078(); virtual void Slot07C();
    virtual void Slot080(); virtual void Slot084(); virtual void Slot088(); virtual void Slot08C();
    virtual void Slot090(); virtual void Slot094(); virtual void Slot098(); virtual void Slot09C();
    virtual void Slot0A0(); virtual void Slot0A4(); virtual void Slot0A8(); virtual void Slot0AC();
    virtual void Slot0B0(); virtual void Slot0B4(); virtual void Slot0B8(); virtual void Slot0BC();
    virtual void Slot0C0(); virtual void Slot0C4(); virtual void Slot0C8(); virtual void Slot0CC();
    virtual void Slot0D0(); virtual void Slot0D4(); virtual void Slot0D8(); virtual void Slot0DC();
    virtual void Slot0E0(); virtual void Slot0E4(); virtual void Slot0E8();
    virtual void ChangeHealth(float amount, bool applyHealthChange);

    void SetCurrentAction(const CCreatureActionBase& action);
};

struct CreatureOverlay
{
    void* vtable;
    unsigned char unused04[0x8D];
    unsigned char flags91;
    unsigned char unused92[0x22];
    float healthB4;
};

class CCreatureAction_StabbedToDeath : public CCreatureActionBase
{
public:
    CCreatureAction_StabbedToDeath(CThingCreatureBase& creature, bool unknown);
    ~CCreatureAction_StabbedToDeath();
private:
    unsigned char storage[0xA8];
};

class CPlayerGui
{
public:
    void AddDamageChange(float amount, bool isDamage);
};
extern CPlayerGui* gPlayerGui;
extern const float gDeathThreshold;
extern const float gDamageThreshold;

class CGameScriptInterface
{
public:
    unsigned char unused00[0x14];
    HeroResolver* heroResolver;
    void ChangeHeroHealthBy(float amount, bool rawHealthMode, bool applyHealthChange) const;
};

void CGameScriptInterface::ChangeHeroHealthBy(
    float amount,
    bool rawHealthMode,
    bool applyHealthChange) const
{
    HeroHandle* handle = heroResolver->ResolveHero();
    if (handle == 0)
        return;
    CThingCreatureBase* hero = handle->item.GetPItem();
    if (hero == 0 || (((CreatureOverlay*)hero)->flags91 & 1) != 0)
        return;

    hero->ChangeHealth(amount, rawHealthMode);
    if (rawHealthMode && ((CreatureOverlay*)hero)->healthB4 < gDeathThreshold)
    {
        hero->SetCurrentAction(CCreatureAction_StabbedToDeath(*hero, false));
    }
    if (*(volatile const bool*)&applyHealthChange && amount < gDamageThreshold)
        gPlayerGui->AddDamageChange(-amount, true);
}