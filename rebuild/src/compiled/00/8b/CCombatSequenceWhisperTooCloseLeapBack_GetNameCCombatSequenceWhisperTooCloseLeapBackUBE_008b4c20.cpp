// CCombatSequenceWhisperTooCloseLeapBack::GetName -> returns CCharString by value.
struct CCharString {
    char* p;
    // ctor at 0x99ebf0: (char const*, int)
    CCharString(const char* s, int n);
};

struct CCombatSequenceWhisperTooCloseLeapBack {
    void* vtbl;
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperTooCloseLeapBack::GetName() const
{
    return CCharString((const char*)0x1278d98, -1);
}