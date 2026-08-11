#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_base(CActiveFile* self) { g_base_called++; (void)self; }
void free_helper(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p)
        free_helper(p);
}

int main() {
    CActiveFile a;
    int dummy = 42;
    a.field0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    g_base_called = 0; g_freed = (void*)0;
    CActiveFile b;
    b.field0 = (void*)0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)0);

    if (ok1 && ok2) printf("007196b0_TEST PASS\n");
    else printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}