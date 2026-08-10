#include <stdio.h>
#include <stdlib.h>

struct CActiveFile {
    void* buf;
};

static int g_helper_called = 0;
static void* g_freed = (void*)0;

extern "C" void __fastcall Helper_OnReadFinished_12(CActiveFile* self)
{
    g_helper_called++;
}

static void model_free(void* p) { g_freed = p; }

void __fastcall OnReadFinished_12(CActiveFile* self)
{
    Helper_OnReadFinished_12(self);
    void* p = self->buf;
    if (p)
        model_free(p);
}

int main()
{
    CActiveFile a;
    int dummy = 0;
    a.buf = &dummy;
    g_helper_called = 0; g_freed = (void*)0;
    OnReadFinished_12(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free-nonnull\n"); return 1; }

    CActiveFile b;
    b.buf = (void*)0;
    g_helper_called = 0; g_freed = (void*)1;
    OnReadFinished_12(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL free-null\n"); return 1; }

    printf("OK_00580a64\n");
    return 0;
}