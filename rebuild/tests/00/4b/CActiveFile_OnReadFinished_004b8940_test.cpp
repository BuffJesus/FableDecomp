#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_baseCalled = 0;
static void* g_freed = (void*)-1;

extern "C" void __fastcall CActiveFile_base(CActiveFile* self) { (void)self; g_baseCalled++; }
extern "C" void __cdecl fable_free(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    // Case 1: field0 non-null -> freed
    int dummy = 7;
    CActiveFile a; a.field0 = &dummy;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_baseCalled == 1) && (g_freed == &dummy);

    // Case 2: field0 null -> not freed
    CActiveFile b; b.field0 = 0;
    g_baseCalled = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_baseCalled == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) printf("004b8940_TEST PASS\n");
    else printf("FAIL b=%d\n", g_baseCalled);
    return 0;
}