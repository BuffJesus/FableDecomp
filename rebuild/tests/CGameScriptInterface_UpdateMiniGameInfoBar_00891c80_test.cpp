#include <cstdio>
struct Inner {
    int calls;
    int last_a, last_b, last_c;
    void method(int a, int b, int c) { calls++; last_a=a; last_b=b; last_c=c; }
};
struct Holder {
    unsigned char pad[0x1c8];
    Inner* p1c8;
};
Holder* g_holder;

void CGameScriptInterface_UpdateMiniGameInfoBar(int param)
{
    g_holder->p1c8->method(param, 0, 0);
}

int main() {
    Inner inner; inner.calls=0; inner.last_a=inner.last_b=inner.last_c=-99;
    Holder h; h.p1c8=&inner; g_holder=&h;
    CGameScriptInterface_UpdateMiniGameInfoBar(1234);
    if (inner.calls==1 && inner.last_a==1234 && inner.last_b==0 && inner.last_c==0) {
        std::printf("CGameScriptInterface_00891c80_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d a=%d b=%d c=%d\n", inner.calls, inner.last_a, inner.last_b, inner.last_c);
    return 1;
}