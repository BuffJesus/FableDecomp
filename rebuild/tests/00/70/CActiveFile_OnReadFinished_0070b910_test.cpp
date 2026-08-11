#include <stdio.h>

struct CActiveFile {
    void *f0;
};

static int g_baseCalled = 0;
static void *g_freed = (void*)0;

void __fastcall CActiveFile_base(CActiveFile *self) { g_baseCalled++; (void)self; }
void freeThing(void *p) { g_freed = p; }

void __fastcall CActiveFile__OnReadFinished(CActiveFile *self);

int main()
{
    // case 1: f0 nonzero -> base called, freeThing gets f0
    CActiveFile a; a.f0 = (void*)0x1234;
    g_baseCalled = 0; g_freed = (void*)0;
    CActiveFile__OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == (void*)0x1234);

    // case 2: f0 zero -> base called, freeThing NOT called
    CActiveFile b; b.f0 = (void*)0;
    g_baseCalled = 0; g_freed = (void*)0xDEAD;
    CActiveFile__OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == (void*)0xDEAD);

    if (ok1 && ok2) printf("0070b910_TEST PASS\n");
    else printf("FAIL b=%d f=%p\n", g_baseCalled, g_freed);
    return 0;
}