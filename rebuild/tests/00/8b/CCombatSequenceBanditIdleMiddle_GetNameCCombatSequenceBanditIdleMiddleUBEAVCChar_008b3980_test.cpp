#include <cstdio>

class CCharString
{
public:
    CCharString(const char* str, long len);
    char* m_pData;
    unsigned long m_Length;
    unsigned long m_Capacity;
};

const char s_BanditIdleMiddleName[] = "CombatSequence_BanditIdleMiddle";

// Capture what the ctor received.
static const char* g_lastStr = 0;
static long g_lastLen = 999;

CCharString::CCharString(const char* str, long len)
{
    g_lastStr = str;
    g_lastLen = len;
    m_pData = (char*)str;
    m_Length = 0;
    m_Capacity = 0;
}

class CCombatSequence_BanditIdleMiddle
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditIdleMiddle::GetName() const
{
    return CCharString(s_BanditIdleMiddleName, -1);
}

int main()
{
    CCombatSequence_BanditIdleMiddle obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastStr == s_BanditIdleMiddleName) && (g_lastLen == -1);
    if (ok)
        printf("BANDIT_GETNAME_OK\n");
    else
        printf("FAIL str=%p len=%ld\n", (void*)g_lastStr, g_lastLen);
    return ok ? 0 : 1;
}