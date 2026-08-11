#include <cstdio>

struct CCharString {
    int d;
    const char* gotLit;
    int gotLen;
    CCharString* ctor(const char* lit, int len) {
        gotLit = lit;
        gotLen = len;
        d = 0x1234;
        return this;
    }
};

CCharString* __fastcall GetActionName(CCharString* thisret) {
    thisret->ctor((const char*)0x123a5cc, -1);
    return thisret;
}

int main() {
    CCharString obj;
    obj.gotLit = 0;
    obj.gotLen = 0;
    CCharString* r = GetActionName(&obj);
    if (r == &obj && obj.gotLit == (const char*)0x123a5cc && obj.gotLen == -1) {
        printf("004d31c0_TEST PASS\n");
    } else {
        printf("FAIL lit=%p len=%d r=%p\n", obj.gotLit, obj.gotLen, (void*)r);
    }
    return 0;
}