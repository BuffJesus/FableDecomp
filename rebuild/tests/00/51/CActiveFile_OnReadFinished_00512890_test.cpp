#include <cstdio>

struct CActiveFile {
    void* p;
};

static int g_base_called = 0;
static void* g_freed = 0;

extern "C" void __fastcall base_510d40(CActiveFile* self) { g_base_called++; (void)self; }
extern "C" void __cdecl free_bfea14(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    int dummy = 42;
    CActiveFile a; a.p = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    g_base_called = 0; g_freed = 0;
    CActiveFile b; b.p = 0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == 0);

    if (ok1 && ok2) printf("00512890_TEST PASS\n");
    else printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}