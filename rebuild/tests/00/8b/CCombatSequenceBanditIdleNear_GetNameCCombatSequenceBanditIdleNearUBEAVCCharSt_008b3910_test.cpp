#include <stdio.h>
#include <string.h>

class CCharString {
public:
    char *m_data;
    CCharString(const char *s, int n);
};

class CCombatSequence_BanditIdleNear {
public:
    virtual CCharString GetName() const;
};

// Local stand-in ctor: record the string it was built from.
static char g_last[64];
CCharString::CCharString(const char *s, int n)
{
    m_data = (char*)s;
    if (s) { strncpy(g_last, s, 63); g_last[63] = 0; }
    else   { g_last[0] = 0; }
    (void)n;
}

CCharString CCombatSequence_BanditIdleNear::GetName() const
{
    return CCharString("BanditIdleNear", -1);
}

int main()
{
    CCombatSequence_BanditIdleNear obj;
    CCharString r = obj.GetName();
    if (r.m_data && strcmp(r.m_data, "BanditIdleNear") == 0 &&
        strcmp(g_last, "BanditIdleNear") == 0) {
        printf("PASS_BANDITIDLE\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}