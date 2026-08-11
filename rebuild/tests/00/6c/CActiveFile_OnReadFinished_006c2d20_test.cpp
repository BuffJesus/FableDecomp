#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_base(CActiveFile* self) { g_base_called++; (void)self; }
void CActiveFile_free(void* p) { g_freed = p; }

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self);

int main()
{
    int dummy;
    CActiveFile a; a.field0 = &dummy;
    g_base_called = 0; g_freed = 0;
    CActiveFile__OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    CActiveFile b; b.field0 = 0;
    g_base_called = 0; g_freed = (void*)1;
    CActiveFile__OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)1);

    if (ok1 && ok2)
        printf("006c2d20_TEST PASS\n");
    else
        printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}