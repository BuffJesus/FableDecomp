#include <cstdio>

// Mocks for the engine callees so the wrapper is testable & deterministic.
static char g_buf[0x54];
static int g_ctor_called = 0;

void* __cdecl engine_operator_new(unsigned int sz)
{
    return (sz == 0x54) ? (void*)g_buf : 0;
}

void* __fastcall obj_ctor_45cdf0(void* thisptr)
{
    g_ctor_called = 1;
    return thisptr;
}

void* __fastcall sub_0045cdf0();

int main()
{
    void* r = sub_0045cdf0();
    if (r == (void*)g_buf && g_ctor_called == 1)
        printf("OK_45cdf0_NEWCTOR\n");
    else
        printf("FAIL_45cdf0\n");
    return 0;
}