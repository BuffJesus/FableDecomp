#include <cstdio>

struct CActiveFile {
    void* buffer;
};

static int g_baseCalled = 0;
static int g_freed = 0;
static void* g_freedPtr = 0;

void __fastcall CActiveFile_Base(CActiveFile* self) { g_baseCalled++; (void)self; }

extern "C" void my_free(void* p) { g_freed++; g_freedPtr = p; }
#define free my_free

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->buffer;
    if (p)
        free(p);
}
#undef free

int main()
{
    int dummy = 0;
    CActiveFile a; a.buffer = &dummy;
    g_baseCalled = g_freed = 0; g_freedPtr = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_baseCalled != 1) { printf("FAIL base\n"); return 1; }
    if (g_freed != 1 || g_freedPtr != &dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b; b.buffer = 0;
    g_baseCalled = g_freed = 0; g_freedPtr = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_baseCalled != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != 0) { printf("FAIL free2\n"); return 1; }

    printf("0075f7a0_TEST PASS\n");
    return 0;
}