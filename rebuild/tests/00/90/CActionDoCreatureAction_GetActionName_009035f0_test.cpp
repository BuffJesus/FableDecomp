#include <stdio.h>
#include <string.h>

// Local stand-in CCharString whose ctor records what it was built with.
class CCharString {
public:
    CCharString(const char *s, long n) { m_p = s; m_n = n; }
    const char *m_p;
    long m_n;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
    int m_dummy;
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}

int main()
{
    CActionDoCreatureAction a;
    a.m_dummy = 0x1234;
    CCharString r = a.GetActionName();
    if (r.m_n == -1 && r.m_p != 0 && strcmp(r.m_p, "DoCreatureAction") == 0) {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("GETACTIONNAME_FAIL\n");
    return 1;
}