#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

static const char* g_lastStr = 0;
static int         g_lastLen = 999;

CCharString::CCharString(const char* s, int len)
{
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

struct CCombatSequence_TrollDoubleSwipeNonMelee {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollDoubleSwipeNonMelee::GetName() const
{
    return CCharString((const char*)0x1278a18, -1);
}

int main()
{
    CCombatSequence_TrollDoubleSwipeNonMelee obj;
    CCharString r = obj.GetName();

    bool ok = (g_lastLen == -1)
           && (g_lastStr == (const char*)0x1278a18)
           && (r.p == (char*)0x1278a18);

    if (ok) {
        printf("GETNAME_TROLL_OK\n");
        return 0;
    }
    printf("GETNAME_TROLL_FAIL len=%d\n", g_lastLen);
    return 1;
}