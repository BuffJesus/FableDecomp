#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CCombatSequence_GenericLeader {
    CCharString GetName() const;
};

static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, int len) {
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

CCharString CCombatSequence_GenericLeader::GetName() const
{
    return CCharString((const char*)0x12786d4, -1);
}

int main()
{
    CCombatSequence_GenericLeader obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastStr == (const char*)0x12786d4)
           && (g_lastLen == -1)
           && (r.p == (char*)0x12786d4);
    if (ok) printf("008b3540_TEST PASS\n");
    else    printf("008b3540_TEST FAIL str=%p len=%d p=%p\n", g_lastStr, g_lastLen, r.p);
    return 0;
}