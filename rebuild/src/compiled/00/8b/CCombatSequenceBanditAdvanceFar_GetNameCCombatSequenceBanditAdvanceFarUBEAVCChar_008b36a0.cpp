// CCombatSequence_BanditAdvanceFar::GetName
// Retail 0x008b36a0: builds a CCharString from a fixed string literal and returns it.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CActionDoCreatureAction;

struct CCombatSequence_BanditAdvanceFar {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAdvanceFar::GetName() const
{
    return CCharString("CombatSequence_BanditAdvanceFar", -1);
}