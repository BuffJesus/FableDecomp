#include <cstdio>
#include <cstring>

struct CCharString {
    char* p;
    int   len;
    CCharString(const char* s, int n) : p((char*)s), len(n) {}
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123bc4c, -1);
}

int main() {
    CActionDoCreatureAction a; a.dummy = 7;
    CCharString r = a.GetActionName();
    if (r.p == (char*)0x0123bc4c && r.len == -1) {
        printf("0077bd00_TEST PASS\n");
    } else {
        printf("FAIL %p %d\n", (void*)r.p, r.len);
    }
    return 0;
}