#include <cstdio>

struct CActiveFile {
    void* buffer;
    void helper();
    void OnReadFinished();
};

static int g_helper_called = 0;
static void* g_freed = 0;

void CActiveFile::helper() { g_helper_called++; }

extern "C" void __cdecl freefn_bfea14(void* p) { g_freed = p; }

int main()
{
    // case 1: non-null buffer -> helper called, freed
    CActiveFile a;
    int dummy = 7;
    a.buffer = &dummy;
    g_helper_called = 0; g_freed = 0;
    a.OnReadFinished();
    if (g_helper_called != 1) { printf("FAIL h1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    // case 2: null buffer -> helper called, not freed
    CActiveFile b;
    b.buffer = 0;
    g_helper_called = 0; g_freed = (void*)1;
    b.OnReadFinished();
    if (g_helper_called != 1) { printf("FAIL h2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL free2\n"); return 1; }

    printf("READFIN_OK\n");
    return 0;
}