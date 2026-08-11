#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int n);
    char* m_data;
    int m_n;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

// Local stand-in for the real ctor at 0x99ebf0.
CCharString::CCharString(const char* s, int n)
{
    m_data = (char*)s;
    m_n = n;
}

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123d56c, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (r.m_data == (char*)0x123d56c && r.m_n == -1)
        printf("OK data=%p n=%d\n", r.m_data, r.m_n);
    else
        printf("FAIL\n");
    return 0;
}