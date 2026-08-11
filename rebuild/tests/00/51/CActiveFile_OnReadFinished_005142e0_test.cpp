#include <cstdio>

struct CActiveFile { void* buffer; };

static int g_baseCalled = 0;
static int g_freed = 0;
static void* g_freedPtr = 0;

void my_free(void* p) { g_freed++; g_freedPtr = p; }

void __fastcall CActiveFile_base(CActiveFile* self) { g_baseCalled++; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    // case 1: non-null buffer -> base called, freed
    int dummy = 0;
    CActiveFile a; a.buffer = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == 1) && (g_freedPtr == &dummy);

    // case 2: null buffer -> base called, NOT freed
    g_baseCalled = 0; g_freed = 0;
    CActiveFile b; b.buffer = 0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == 0);

    if (ok1 && ok2) printf("005142e0_TEST PASS\n");
    else printf("FAIL b=%d f=%d\n", g_baseCalled, g_freed);
    return 0;
}