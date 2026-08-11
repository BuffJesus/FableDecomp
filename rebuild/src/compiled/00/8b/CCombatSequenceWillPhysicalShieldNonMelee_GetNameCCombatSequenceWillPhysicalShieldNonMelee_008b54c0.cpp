// CCombatSequence_WillPhysicalShieldNonMelee::GetName  @ retail 0x008b54c0
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequence_WillPhysicalShieldNonMelee {
    virtual CCharString GetName();
};

CCharString CCombatSequence_WillPhysicalShieldNonMelee::GetName()
{
    return CCharString((const char*)0x1278fb8, -1);
}