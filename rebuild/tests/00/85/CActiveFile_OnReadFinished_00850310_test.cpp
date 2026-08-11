#include <cstdio>

struct CActiveFile { void* m_buf; };

static int g_baseCalls = 0;
static int g_freeCalls = 0;
static void* g_freed = 0;

void __fastcall base_helper_850280(void* self) { (void)self; ++g_baseCalls; }
void __cdecl free_bfea14(void* p) { ++g_freeCalls; g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self, int edx);

int main()
{
    // Case 1: non-null buffer -> base called, free called on that buffer
    int dummy = 7;
    CActiveFile a; a.m_buf = &dummy;
    g_baseCalls = g_freeCalls = 0; g_freed = 0;
    CActiveFile_OnReadFinished(&a, 0);
    bool ok1 = (g_baseCalls == 1) && (g_freeCalls == 1) && (g_freed == &dummy);

    // Case 2: null buffer -> base called, free NOT called
    CActiveFile b; b.m_buf = 0;
    g_baseCalls = g_freeCalls = 0; g_freed = 0;
    CActiveFile_OnReadFinished(&b, 0);
    bool ok2 = (g_baseCalls == 1) && (g_freeCalls == 0);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL b=%d f=%d\n", g_baseCalls, g_freeCalls);
    return 1;
}