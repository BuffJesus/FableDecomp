// CCombatSequence_ScorpionLungeAttack::GetName
// retail 0x008b3ea0 : returns CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    // ctor(const char*, long) at retail 0x0099ebf0
    CCharString(const char* s, long n);
};

// The literal string lives at 0x12789a0 in retail; model as an extern data pointer
// so the compiler emits a single push of its address.
extern const char kScorpionLungeName[];

struct CActionDoCreatureAction;

struct CCombatSequence_ScorpionLungeAttack {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_ScorpionLungeAttack::GetName() const
{
    return CCharString(kScorpionLungeName, -1);
}