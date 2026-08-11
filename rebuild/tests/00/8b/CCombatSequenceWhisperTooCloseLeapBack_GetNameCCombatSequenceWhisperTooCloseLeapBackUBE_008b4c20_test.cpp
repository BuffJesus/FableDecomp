#include <cstdio>
#include <cstring>

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CCombatSequenceWhisperTooCloseLeapBack {
    void* vtbl;
    CCharString GetName() const;
};

// Local stand-in for the real CCharString ctor at 0x99ebf0.
static const char* g_lastStr;
static int g_lastN;
CCharString::CCharString(const char* s, int n)
{
    g_lastStr = s;
    g_lastN = n;
    p = (char*)s;
}

CCharString CCombatSequenceWhisperTooCloseLeapBack::GetName() const
{
    return CCharString((const char*)0x1278d98, -1);
}

int main()
{
    CCombatSequenceWhisperTooCloseLeapBack obj;
    obj.vtbl = 0;
    CCharString r = obj.GetName();
    if (g_lastStr == (const char*)0x1278d98 && g_lastN == -1 && r.p == (char*)0x1278d98) {
        printf("LEAPBACK_GETNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}