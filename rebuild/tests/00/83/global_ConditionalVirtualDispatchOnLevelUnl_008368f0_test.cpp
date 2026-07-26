#include <cstdio>

int g_enabledFlag = 1;
int g_called = 0;
int g_arg = 0;

struct Target {
    virtual void v0(int) {}
    virtual void v1(int) {}
    virtual void v2(int) {}
    virtual void v3(int) {}
    virtual void v4(int) {}
    virtual void v5(int) {}
    virtual void v6(int) {}
    virtual void v7(int) {}
    virtual void v8(int) {}
    virtual void v9(int) {}
    virtual void v10(int) {}
    virtual void v11(int) {}
    virtual void v12(int) {}
    virtual void OnLevelUnload(int a) { g_called = 1; g_arg = a; }
};

struct Owner {
    char pad[0x91];
    unsigned char flags;
    char pad2[0x98 - 0x92];
    Target* target;
    void dispatch(int arg);
};

void Owner::dispatch(int arg)
{
    if (g_enabledFlag == 0) return;
    if (this->flags & 0x1) return;
    this->target->OnLevelUnload(arg);
}

int main()
{
    Target t;
    Owner o;
    o.flags = 0;
    o.target = &t;

    g_enabledFlag = 1; g_called = 0; g_arg = 0;
    o.dispatch(42);
    if (!g_called || g_arg != 42) { std::printf("FAIL dispatch\n"); return 1; }

    g_called = 0;
    o.flags = 1;
    o.dispatch(7);
    if (g_called) { std::printf("FAIL flag-set should skip\n"); return 1; }

    g_called = 0;
    o.flags = 0;
    g_enabledFlag = 0;
    o.dispatch(9);
    if (g_called) { std::printf("FAIL disabled should skip\n"); return 1; }

    std::printf("_global_008368f0_TEST PASS\n");
    return 0;
}