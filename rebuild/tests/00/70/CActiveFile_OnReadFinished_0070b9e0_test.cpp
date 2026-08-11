#include <cstdio>

struct CActiveFile {
    void* buffer;
};

static int g_base_called = 0;
static void* g_freed = (void*)-1;

extern "C" void __fastcall CActiveFile_base_Cleanup(CActiveFile* self)
{
    (void)self;
    g_base_called++;
}

extern "C" void __cdecl fable_free(void* p)
{
    g_freed = p;
}

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    // Case 1: buffer non-null -> base called, freed
    int dummy;
    CActiveFile a;
    a.buffer = &dummy;
    g_base_called = 0;
    g_freed = (void*)-1;
    OnReadFinished(&a);
    if (g_base_called != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    // Case 2: buffer null -> base called, NOT freed
    CActiveFile b;
    b.buffer = 0;
    g_base_called = 0;
    g_freed = (void*)-1;
    OnReadFinished(&b);
    if (g_base_called != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)-1) { printf("FAIL free2\n"); return 1; }

    printf("0070b9e0_TEST PASS\n");
    return 0;
}