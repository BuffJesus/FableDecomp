#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int len);
    void* m_data;
};

static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, int len) {
    g_lastStr = s; g_lastLen = len; m_data = (void*)s;
}

class CCombatSequence_KingScorpionTailPlunge {
public:
    virtual CCharString GetName();
    int m_pad;
};

CCharString CCombatSequence_KingScorpionTailPlunge::GetName()
{
    return CCharString("CombatSequence_KingScorpionTailPlunge", -1);
}

int main()
{
    CCombatSequence_KingScorpionTailPlunge obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastStr != 0)
        && (std::strcmp(g_lastStr, "CombatSequence_KingScorpionTailPlunge") == 0)
        && (g_lastLen == -1)
        && (r.m_data == (void*)g_lastStr);
    if (ok) { std::printf("KINGSCORPION_GETNAME_OK\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}