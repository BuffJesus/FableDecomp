#include <stdio.h>

static int g_arg = -99;
static int g_called = 0;

struct Obj {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4(int a) {}   // vtbl+0x10
};

void __stdcall EditFrameUpdate(Obj* p)
{
    if (p) {
        p->v4(1);
    }
}

struct Impl : public Obj {
    void v4(int a) { g_called++; g_arg = a; }
};

int main()
{
    // null path: nothing happens
    EditFrameUpdate(0);
    if (g_called != 0) { printf("FAIL null path called\n"); return 0; }

    Impl obj;
    EditFrameUpdate(&obj);
    if (g_called == 1 && g_arg == 1) {
        printf("00c1b930_TEST PASS\n");
    } else {
        printf("FAIL called=%d arg=%d\n", g_called, g_arg);
    }
    return 0;
}