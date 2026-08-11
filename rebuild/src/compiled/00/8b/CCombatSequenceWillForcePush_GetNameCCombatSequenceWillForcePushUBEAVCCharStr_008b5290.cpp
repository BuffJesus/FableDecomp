// CCombatSequence_WillForcePush::GetName  @ 0x008b5290
// virtual CCharString GetName(void) const  -- returns a CCharString by value
// built from a string literal via the CCharString(char const*, int) ctor @ 0x0099ebf0.

class CCharString {
public:
    CCharString(const char* s, int len);   // 0x0099ebf0
    CCharString(const CCharString& o);
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

class CActionDoCreatureAction;

class CCombatSequence_WillForcePush {
public:
    virtual CCharString GetName(void) const;
};

CCharString CCombatSequence_WillForcePush::GetName(void) const
{
    return CCharString("CombatSequence_WillForcePush", -1);
}