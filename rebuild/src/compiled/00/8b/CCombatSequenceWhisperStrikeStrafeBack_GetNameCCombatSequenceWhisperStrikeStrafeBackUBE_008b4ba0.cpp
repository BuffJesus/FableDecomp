// CCombatSequenceWhisperStrikeStrafeBack::GetName  @ retail 0x008b4ba0
// Returns a CCharString by value built from a string literal.

struct CCharString {
    char* m_data;
    CCharString();
    CCharString(const char* str, int len);
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CCombatSequenceWhisperStrikeStrafeBack {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperStrikeStrafeBack::GetName()
{
    return CCharString((const char*)0x1278d6c, -1);
}