// CCombatSequence_WillLightning::GetName  (retail 0x008b5120)
// __fastcall, returns CCharString by value from a string literal.

class CCharString {
public:
    char* m_ptr;
    // out-of-line ctor -> retail CCharString::CCharString(const char*, int)
    CCharString(const char* s, int len);
};

class CCombatSequence_WillLightning {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_WillLightning::GetName() const
{
    return CCharString("CombatSequence_WillLightning", -1);
}