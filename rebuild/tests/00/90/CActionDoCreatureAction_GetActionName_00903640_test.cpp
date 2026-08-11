#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_data;
    const char* m_src;
    int m_n;
    CCharString(const char* s, int n) { m_data = 0; m_src = s; m_n = n; }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("CACTION_DO_CREATURE_ACTION", -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (r.m_n == -1 && r.m_src && std::strcmp(r.m_src, "CACTION_DO_CREATURE_ACTION") == 0) {
        std::printf("GETACTIONNAME_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}