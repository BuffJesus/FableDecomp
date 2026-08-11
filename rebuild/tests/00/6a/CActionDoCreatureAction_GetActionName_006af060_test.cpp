#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len) { p = (char*)s; last_len = len; }
    static int last_len;
    static const char* last_s;
};
int CCharString::last_len = 0;
const char* CCharString::last_s = 0;

// redefine ctor to capture (single TU test)
class CActionDoCreatureAction {
    int dummy;
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123c110, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    bool ok = (r.p == (char*)0x123c110);
    if (ok) printf("006af060_TEST PASS\n");
    else printf("FAIL p=%p\n", r.p);
    return 0;
}