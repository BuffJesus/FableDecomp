#include <cstdio>

static const char* g_lastS;
static int g_lastLen;

struct CCharString {
    void* p;
    CCharString(const char* s, int len);
};

// Local stand-in for the ctor at 0x0099ebf0.
CCharString::CCharString(const char* s, int len)
{
    g_lastS = s;
    g_lastLen = len;
    p = (void*)s;
}

struct CCombatSequence_TrollOpeningGroundPound {
    CCharString GetName() const;
};

CCharString CCombatSequence_TrollOpeningGroundPound::GetName() const
{
    return CCharString((const char*)0x1278a48, -1);
}

int main()
{
    CCombatSequence_TrollOpeningGroundPound obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastS == (const char*)0x1278a48)
           && (g_lastLen == -1)
           && (r.p == (void*)0x1278a48);
    if (ok) printf("GETNAME_GROUNDPOUND_OK\n");
    else    printf("FAIL s=%p len=%d p=%p\n", g_lastS, g_lastLen, r.p);
    return ok ? 0 : 1;
}