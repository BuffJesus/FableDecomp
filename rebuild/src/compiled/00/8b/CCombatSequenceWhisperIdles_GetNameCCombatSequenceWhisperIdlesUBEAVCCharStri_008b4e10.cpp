// CCombatSequenceWhisperIdles::GetName  @ retail 0x008b4e10
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequenceWhisperIdles {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperIdles::GetName()
{
    return CCharString((const char*)0x1278e38, -1);
}