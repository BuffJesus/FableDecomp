#include <cstdio>

struct CActiveFile { void* field0; };

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall base_0x62e2b0(CActiveFile* self) { g_base_called++; }
void free_0xbfea14(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    base_0x62e2b0(self);
    void* p = *(void**)self;
    if (p) { free_0xbfea14(p); }
}

int main()
{
    int dummy = 5;
    CActiveFile a; a.field0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    g_base_called = 0; g_freed = (void*)0;
    CActiveFile b; b.field0 = (void*)0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)0);

    if (ok1 && ok2) printf("0062e8e0_TEST PASS\n");
    else printf("FAIL %d %d\n", (int)ok1, (int)ok2);
    return 0;
}