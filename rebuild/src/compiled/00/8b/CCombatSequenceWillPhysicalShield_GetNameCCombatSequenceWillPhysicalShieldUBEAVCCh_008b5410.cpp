// CCombatSequence_WillPhysicalShield::GetName  @ retail 0x008b5410
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequence_WillPhysicalShield {
    virtual CCharString GetName();
};

CCharString CCombatSequence_WillPhysicalShield::GetName()
{
    return CCharString((const char*)0x1278f90, -1);
}