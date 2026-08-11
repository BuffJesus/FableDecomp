#include <stdio.h>

struct CCharString {
    int d;
    const char* saved_s;
    int saved_n;
    CCharString(const char* s, int n) { d = 0; saved_s = s; saved_n = n; }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c168, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.saved_s == (const char*)0x125c168 && r.saved_n == -1)
        printf("00692250_TEST PASS\n");
    else
        printf("FAIL s=%p n=%d\n", r.saved_s, r.saved_n);
    return 0;
}