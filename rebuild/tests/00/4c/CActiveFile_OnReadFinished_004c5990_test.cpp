#include <cstdio>

struct CActiveFile {
    void* buf;
};

static int g_base_called = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_base(CActiveFile* self) { g_base_called++; (void)self; }
void free_bfea14(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->buf;
    if (p)
        free_bfea14(p);
}

int main()
{
    int dummy = 0;
    CActiveFile a; a.buf = &dummy;
    g_base_called = 0; g_freed = 0;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_freed == &dummy);

    CActiveFile b; b.buf = 0;
    g_base_called = 0; g_freed = (void*)1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_freed == (void*)1); // free not called

    if (ok1 && ok2)
        printf("004c5990_TEST PASS\n");
    else
        printf("FAIL b=%d f=%p\n", g_base_called, g_freed);
    return 0;
}