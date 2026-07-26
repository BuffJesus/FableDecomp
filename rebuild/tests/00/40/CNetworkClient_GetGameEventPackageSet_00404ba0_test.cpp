#include <cstdio>

struct CNetworkClient {
    char pad0[8];
    char flag8;
};

static int g_called = 0;
bool __fastcall CNetworkClient_GetGameEventPackageSet_delegate(CNetworkClient* self, int arg)
{
    (void)self; (void)arg;
    g_called = 1;
    return false;
}

bool __fastcall CNetworkClient_GetGameEventPackageSet(CNetworkClient* self, int arg)
{
    if (self->flag8)
        return CNetworkClient_GetGameEventPackageSet_delegate(self, arg);
    return true;
}

int main()
{
    CNetworkClient c;
    c.flag8 = 0;
    g_called = 0;
    bool r1 = CNetworkClient_GetGameEventPackageSet(&c, 42);
    if (r1 != true || g_called != 0) { std::printf("FAIL branch-false\n"); return 1; }

    c.flag8 = 1;
    g_called = 0;
    bool r2 = CNetworkClient_GetGameEventPackageSet(&c, 42);
    if (r2 != false || g_called != 1) { std::printf("FAIL branch-true\n"); return 1; }

    std::printf("CNetworkClient_00404ba0_TEST PASS\n");
    return 0;
}