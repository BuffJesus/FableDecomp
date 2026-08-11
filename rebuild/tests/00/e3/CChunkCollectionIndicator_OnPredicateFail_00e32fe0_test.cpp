#include <cstdio>

// Vtable model matching s.cpp: slot 335 is fn(int,int,int)
struct CalleeVtbl {
    void* slots[335];
    void (__fastcall *fn)(void* self, int edx, int a, int b, int c);
};

struct Callee {
    const CalleeVtbl* vtbl;
};

struct CChunkCollectionIndicator {
    char pad00[0x40];
    Callee* f40;
    int     f44;
    int     f48;
    char    pad4c[0x5c-0x4c];
    int     f5c;
    void OnPredicateFail();
};

static int g_a, g_b, g_c;
static void* g_self;

static void __fastcall recorder(void* self, int /*edx*/, int a, int b, int c)
{
    g_self = self; g_a = a; g_b = b; g_c = c;
}

int main()
{
    CalleeVtbl vt;
    for (int i = 0; i < 335; ++i) vt.slots[i] = 0;
    vt.fn = recorder;

    Callee cal;
    cal.vtbl = &vt;

    CChunkCollectionIndicator ind;
    ind.f40 = &cal;
    ind.f48 = 0x1234;
    ind.f5c = 0x5678;

    ind.OnPredicateFail();

    if (g_self == &cal && g_a == 0x5678 && g_b == 0x1234 && g_c == -1)
        printf("PREDFAIL_OK\n");
    else
        printf("PREDFAIL_BAD a=%x b=%x c=%x\n", g_a, g_b, g_c);
    return 0;
}