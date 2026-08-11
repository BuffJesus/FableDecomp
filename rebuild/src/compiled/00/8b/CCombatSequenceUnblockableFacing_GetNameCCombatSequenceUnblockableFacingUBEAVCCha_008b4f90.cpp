// CCombatSequenceUnblockableFacing::GetName  @ retail 0x008b4f90
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequenceUnblockableFacing {
    virtual CCharString GetName();
};

CCharString CCombatSequenceUnblockableFacing::GetName()
{
    return CCharString((const char*)0x1278e90, -1);
}