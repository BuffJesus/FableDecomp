// CCombatSequence_FodderMainAttack::GetName
// Returns a CCharString by value built from a static string literal.
// Retail: push -1; push 0x1278900; ecx=retbuf; call CCharString::ctor; return retbuf.

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

struct CActionDoCreatureAction;

class CCombatSequence_FodderMainAttack {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_FodderMainAttack::GetName() const
{
    return CCharString("FodderMainAttack", -1);
}