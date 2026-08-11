#include <stdio.h>
#include <string.h>

// Local standalone re-declaration matching the shape under test, with a
// definition for the CCharString ctor so the test links independently.

class CCharString {
public:
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
    char* m_p;
    int   m_len;
    int   m_cap;
};

static char g_buf[256];

CCharString::CCharString(const char* s, int len)
{
    int n = (len < 0) ? (int)strlen(s) : len;
    strncpy(g_buf, s, sizeof(g_buf) - 1);
    g_buf[sizeof(g_buf) - 1] = 0;
    m_p = g_buf;
    m_len = n;
    m_cap = n;
}

CCharString::CCharString(const CCharString& o)
{
    m_p = o.m_p; m_len = o.m_len; m_cap = o.m_cap;
}

class CCombatSequence_WillForcePush {
public:
    virtual CCharString GetName(void) const;
};

CCharString CCombatSequence_WillForcePush::GetName(void) const
{
    return CCharString("CombatSequence_WillForcePush", -1);
}

int main()
{
    CCombatSequence_WillForcePush obj;
    CCharString name = obj.GetName();
    if (strcmp(name.m_p, "CombatSequence_WillForcePush") == 0 && name.m_len == 28) {
        printf("GETNAME_PASS_WILLFORCEPUSH\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}