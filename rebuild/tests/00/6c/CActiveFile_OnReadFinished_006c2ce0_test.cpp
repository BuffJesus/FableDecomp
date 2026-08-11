#include <cstdio>

struct CActiveFile {
    void* m_ptr;
};

static int g_base_called = 0;
static void* g_freed = 0;

void __fastcall BaseOnReadFinished(CActiveFile* self) { (void)self; g_base_called++; }
extern "C" void FreeReleased(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    static int storage[4];
    void* pmarker = (void*)storage;
    CActiveFile a; a.m_ptr = pmarker;
    CActiveFile b; b.m_ptr = 0;
    CActiveFile* pa = &a;
    CActiveFile* pb = &b;

    g_base_called = 0; g_freed = 0;
    CActiveFile_OnReadFinished(pa);
    bool ok1 = (g_base_called == 1) && (g_freed == pmarker);

    g_base_called = 0; g_freed = 0;
    CActiveFile_OnReadFinished(pb);
    bool ok2 = (g_base_called == 1) && (g_freed == 0);

    if (ok1 && ok2) printf("006c2ce0_TEST PASS\n");
    else printf("FAIL b=%d f1=%p\n", g_base_called, g_freed);
    return 0;
}