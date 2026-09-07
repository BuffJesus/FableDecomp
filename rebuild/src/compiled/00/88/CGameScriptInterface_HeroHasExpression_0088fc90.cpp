class CCharString;

class CThingPlayerCreature
{
public:
    bool IsExpressionAvailable(const CCharString& expression);
};

class HeroResolverStage2;

class HeroResolverStage1
{
public:
    HeroResolverStage2* ResolveStage2();
};

class HeroResolverStage2
{
public:
    CThingPlayerCreature* ResolveHero();
};

class CGameScriptInterface
{
public:
    bool HeroHasExpression(const CCharString& expression) const;
};

struct InterfaceOverlay
{
    unsigned char pad00[0x14];
    HeroResolverStage1* field14;
};

struct HeroOverlay
{
    unsigned char pad00[0x91];
    unsigned char flags91;
};

bool CGameScriptInterface::HeroHasExpression(
    const CCharString& expression) const
{
    const InterfaceOverlay* self = (const InterfaceOverlay*)this;
    CThingPlayerCreature* hero = self->field14->ResolveStage2()->ResolveHero();
    if (hero != 0 && (((const HeroOverlay*)hero)->flags91 & 1) == 0)
    {
        return hero->IsExpressionAvailable(expression);
    }
    return false;
}