#include <cstdio>
#include <cstring>

class CCharString
{
public:
    char* m_data;
    CCharString(const char* s, int len);
};

// local stand-in for the engine ctor @ 0x99ebf0
CCharString::CCharString(const char* s, int len)
{
    int n = (len < 0) ? (int)strlen(s) : len;
    m_data = new char[n + 1];
    memcpy(m_data, s, n);
    m_data[n] = 0;
}

class CActionDoCreatureAction { };

CCharString __fastcall CActionDoCreatureAction_GetActionName(CActionDoCreatureAction* /*self*/)
{
    return CCharString("DoCreatureAction", -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString s = CActionDoCreatureAction_GetActionName(&a);
    if (strcmp(s.m_data, "DoCreatureAction") == 0)
        printf("GETACTIONNAME_PASS\n");
    else
        printf("FAIL:%s\n", s.m_data);
    return 0;
}