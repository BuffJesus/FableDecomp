#include <stdio.h>

struct CActiveFile { void* m_ptr; };

static int g_baseCalled = 0;
static void* g_freed = 0;

extern "C" void __fastcall CActiveFile_base_0x70ba00(CActiveFile* self) { (void)self; g_baseCalled++; }
extern "C" void CActiveFile_free_0xbfea14(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    int dummy = 0;
    CActiveFile a; a.m_ptr = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == &dummy);

    g_baseCalled = 0; g_freed = (void*)1;
    CActiveFile b; b.m_ptr = 0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == (void*)1); // not freed

    if (ok1 && ok2) printf("0070ba70_TEST PASS\n");
    else printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    return 0;
}