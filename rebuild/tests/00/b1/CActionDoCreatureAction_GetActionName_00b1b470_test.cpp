#include <stdio.h>
#include <string.h>

struct CCharString {
    char* m_data;
    CCharString(const char* s, int n);
};

char g_actionName_str[] = "DoCreatureAction";

// simple length-computing ctor: n==-1 means strlen
CCharString::CCharString(const char* s, int n)
{
    m_data = (char*)s;
    (void)n;
}

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(g_actionName_str, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (r.m_data == g_actionName_str && strcmp(r.m_data, "DoCreatureAction") == 0) {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}