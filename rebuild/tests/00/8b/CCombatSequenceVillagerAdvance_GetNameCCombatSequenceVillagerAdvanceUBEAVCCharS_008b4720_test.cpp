#include <cstdio>

// ---- reproduce the source under test (self-contained) ----
class CCharString
{
public:
    CCharString(const char* str, int len);
    const char* data() const { return m_data; }
private:
    char* m_data;
};

// Local stand-in for CCharString::CCharString(const char*, int) @ 0x0099ebf0.
static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* str, int len)
{
    g_lastStr = str;
    g_lastLen = len;
    m_data = (char*)str;
}

class CCombatSequence_VillagerAdvance
{
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_VillagerAdvance::GetName() const
{
    return CCharString((const char*)0x1278c38, -1);
}

int main()
{
    CCombatSequence_VillagerAdvance obj;
    CCharString s = obj.GetName();

    bool ok = true;
    if (g_lastStr != (const char*)0x1278c38) ok = false;
    if (g_lastLen != -1) ok = false;
    if (s.data() != (const char*)0x1278c38) ok = false;

    if (ok)
        printf("GETNAME_VILLAGERADVANCE_OK\n");
    else
        printf("GETNAME_FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);

    return ok ? 0 : 1;
}