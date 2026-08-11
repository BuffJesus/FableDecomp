#include <cstdio>

struct CCharString {
    int d;
    const char* gotLit;
    int gotLen;
    CCharString(const char* lit, int len) {
        gotLit = lit;
        gotLen = len;
        d = 0x1234;
    }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x123d538, -1);
}

int main() {
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.gotLit == (const char*)0x123d538 && r.gotLen == -1) {
        printf("007d4d70_TEST PASS\n");
    } else {
        printf("FAIL lit=%p len=%d\n", r.gotLit, r.gotLen);
    }
    return 0;
}