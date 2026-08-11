// CCombatSequence_WillEnflame::GetName  (retail 0x008b51d0)
// virtual CCharString GetName() const;  -- returns a CCharString by value built
// from a fixed string literal via CCharString::CCharString(char const*, int).

class CCharString {
public:
    // ctor at 0x0099ebf0 : CCharString(char const* s, int n)
    CCharString(const char* s, int n);
private:
    char* m_p;
};

class CCombatSequence_WillEnflame {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillEnflame::GetName() const
{
    return CCharString("SEQ_HERO_WILL_ENFLAME", -1);
}