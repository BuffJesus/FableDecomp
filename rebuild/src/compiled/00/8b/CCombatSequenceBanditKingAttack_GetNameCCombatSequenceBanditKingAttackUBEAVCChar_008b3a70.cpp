// CCombatSequence_BanditKingAttack::GetName  @ 008b3a70
// Returns a CCharString constructed from a fixed string literal.

class CCharString {
public:
    CCharString(const char* s, int n);
    char* m_data;
};

// helper at 0x99ebf0: CCharString::CCharString(char const*, int)
// modelled via the ctor above.

struct CActionDoCreatureAction;

class CCombatSequence_BanditKingAttack {
public:
    virtual CCharString GetName() const;
    // padding to keep it a polymorphic type; not otherwise used
};

CCharString CCombatSequence_BanditKingAttack::GetName() const
{
    return CCharString((const char*)0x1278868, -1);
}