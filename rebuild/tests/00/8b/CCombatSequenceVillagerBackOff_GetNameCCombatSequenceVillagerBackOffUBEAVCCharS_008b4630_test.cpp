#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_pData;
    CCharString(const char* str, int len);
};

static char g_buf[256];
CCharString::CCharString(const char* str, int len)
{
    // emulate retail ctor: copy the literal (len<0 => strlen)
    if (len < 0) len = (int)std::strlen(str);
    std::memcpy(g_buf, str, len);
    g_buf[len] = 0;
    m_pData = g_buf;
}

struct CActionDoCreatureAction {
    virtual CCharString GetName() const;
};

CCharString CActionDoCreatureAction::GetName() const
{
    return CCharString("CombatSequence_VillagerBackOff", -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString s = a.GetName();
    if (std::strcmp(s.m_pData, "CombatSequence_VillagerBackOff") == 0) {
        std::printf("GETNAME_OK\n");
        return 0;
    }
    std::printf("GETNAME_FAIL\n");
    return 1;
}