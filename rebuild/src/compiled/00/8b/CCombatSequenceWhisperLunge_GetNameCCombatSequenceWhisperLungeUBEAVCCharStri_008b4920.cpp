// CCombatSequenceWhisperLunge::GetName  @ retail 0x008b4920
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequenceWhisperLunge {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperLunge::GetName()
{
    return CCharString((const char*)0x1278ce0, -1);
}