#include <cstdio>

struct CCharString {
    const char* s;
    int n;
    CCharString(const char* str, int cnt) : s(str), n(cnt) {}
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x126554c, -1);
}

int main() {
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.s == (const char*)0x126554c && r.n == -1) {
        printf("00718610_TEST PASS\n");
    } else {
        printf("FAIL s=%p n=%d\n", r.s, r.n);
    }
    return 0;
}