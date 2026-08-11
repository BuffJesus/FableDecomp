#include <cstdio>

// ---- standalone model of the CCharString + ctor + method under test ----

struct CCharString
{
    const char* p;
    int         len;
    void Construct(const char* s, int length);   // stand-in for retail 0x0099ebf0
};

void CCharString::Construct(const char* s, int length)
{
    p   = s;
    len = length;
}

struct CCombatSequenceWhisperBoast
{
    virtual CCharString GetName() const;
};

CCharString CCombatSequenceWhisperBoast::GetName() const
{
    CCharString result;
    result.Construct(reinterpret_cast<const char*>(0x1278dc4), -1);
    return result;
}

int main()
{
    CCombatSequenceWhisperBoast obj;
    CCharString r = obj.GetName();

    bool ok = (r.p == reinterpret_cast<const char*>(0x1278dc4)) && (r.len == -1);

    if (ok)
        printf("WHISPERBOAST_GETNAME_OK\n");
    else
        printf("FAIL p=%p len=%d\n", (void*)r.p, r.len);

    return ok ? 0 : 1;
}