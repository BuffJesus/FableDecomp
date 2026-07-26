#include <cstdio>

struct CIME { char pad[8]; char enabled; };

static int g_called = 0;
static void* g_a = 0;
static int g_b = 0;

void __fastcall CIME_helper(void* p, int q)
{
    g_called = 1;
    g_a = p;
    g_b = q;
}

void __fastcall CIME_FinaliseString(CIME* self, int edx_ignore, void* a, int b)
{
    if (self->enabled)
        CIME_helper(a, b);
}

int main()
{
    CIME c;
    for (int i = 0; i < 9; ++i) ((char*)&c)[i] = 0;

    // enabled == 0 -> helper NOT called
    c.enabled = 0;
    g_called = 0;
    int marker = 0;
    CIME_FinaliseString(&c, 0, &marker, 42);
    if (g_called != 0) { std::printf("FAIL: called when disabled\n"); return 1; }

    // enabled != 0 -> helper called with args
    c.enabled = 1;
    g_called = 0;
    CIME_FinaliseString(&c, 0, &marker, 99);
    if (g_called != 1) { std::printf("FAIL: not called when enabled\n"); return 1; }
    if (g_a != &marker) { std::printf("FAIL: wrong ptr arg\n"); return 1; }
    if (g_b != 99) { std::printf("FAIL: wrong int arg\n"); return 1; }

    std::printf("CIME_00404b80_TEST PASS\n");
    return 0;
}