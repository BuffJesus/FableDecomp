#include <cstdio>

static int g_freed = 0;
static void* g_last = 0;

extern "C" void __cdecl free_block(void* p) { g_freed++; g_last = p; }

struct CSystemRegistry {
    void* field0;
    void* field4;
    void* field8;
    void* fieldC;
    void* field10;
};

void __fastcall CSystemRegistry_dtor(CSystemRegistry* self)
{
    if (self->field10)
        free_block(self->field10);
    if (self->field0)
        free_block(self->field0);
}

int main()
{
    CSystemRegistry a;
    a.field0 = (void*)0x1111;
    a.field10 = (void*)0x2222;
    g_freed = 0;
    CSystemRegistry_dtor(&a);
    if (g_freed != 2) { std::printf("FAIL count=%d\n", g_freed); return 1; }

    CSystemRegistry b;
    b.field0 = 0;
    b.field10 = 0;
    g_freed = 0;
    CSystemRegistry_dtor(&b);
    if (g_freed != 0) { std::printf("FAIL count2=%d\n", g_freed); return 1; }

    CSystemRegistry c;
    c.field0 = (void*)0x3333;
    c.field10 = 0;
    g_freed = 0;
    CSystemRegistry_dtor(&c);
    if (g_freed != 1) { std::printf("FAIL count3=%d\n", g_freed); return 1; }

    std::printf("CSystemRegistry_0045645d_TEST PASS\n");
    return 0;
}