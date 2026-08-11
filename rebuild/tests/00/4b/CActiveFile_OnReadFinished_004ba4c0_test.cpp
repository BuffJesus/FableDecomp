#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_baseCalls = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_base_handler(CActiveFile* self) { (void)self; ++g_baseCalls; }
void FreeHelper(void* p) { g_freed = p; }

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self);

int main()
{
    int dummy = 0;
    CActiveFile a; a.field0 = &dummy;
    g_baseCalls = 0; g_freed = (void*)0;
    CActiveFile__OnReadFinished(&a);
    if (g_baseCalls != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL freed1\n"); return 1; }

    CActiveFile b; b.field0 = (void*)0;
    g_baseCalls = 0; g_freed = (void*)1;
    CActiveFile__OnReadFinished(&b);
    if (g_baseCalls != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL freed2\n"); return 1; }

    printf("004ba4c0_TEST PASS\n");
    return 0;
}