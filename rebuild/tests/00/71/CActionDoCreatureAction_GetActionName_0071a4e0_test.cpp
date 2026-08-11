#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len) { p = (char*)s; last_len = len; }
    static int last_len;
};
int CCharString::last_len = 0;

class CActionDoCreatureAction {
    int dummy;
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x12659e4, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    bool ok = (r.p == (char*)0x12659e4) && (CCharString::last_len == -1);
    if (ok) printf("0071a4e0_TEST PASS\n");
    else printf("FAIL p=%p len=%d\n", r.p, CCharString::last_len);
    return 0;
}