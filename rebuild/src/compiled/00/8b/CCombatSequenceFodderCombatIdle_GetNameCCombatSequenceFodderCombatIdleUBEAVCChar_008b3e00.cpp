// CCombatSequence_FodderCombatIdle::GetName
// Returns a CCharString by value built from a static string literal.
// Retail: push -1; push 0x127897c; ecx=retbuf; call CCharString::ctor; return retbuf.

class CCharString {
public:
    CCharString(const char* s, int len);
    char* m_data;
};

struct CActionDoCreatureAction;

class CCombatSequence_FodderCombatIdle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_FodderCombatIdle::GetName() const
{
    return CCharString("FodderCombatIdle", -1);
}