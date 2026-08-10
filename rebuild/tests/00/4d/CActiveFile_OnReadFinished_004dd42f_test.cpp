#include <stdio.h>

struct CActiveFile { void* field0; };

static int g_helper_called = 0;
static void* g_freed = 0;

void __fastcall Helper_4dbad3(CActiveFile* self) { g_helper_called++; (void)self; }
void Free_bfea14(void* p) { g_freed = p; }

void __fastcall OnReadFinished_10(CActiveFile* self)
{
    Helper_4dbad3(self);
    void* p = self->field0;
    if (p)
        Free_bfea14(p);
}

int main()
{
    int dummy = 5;
    CActiveFile a; a.field0 = &dummy;
    g_helper_called = 0; g_freed = 0;
    OnReadFinished_10(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b; b.field0 = 0;
    g_helper_called = 0; g_freed = (void*)1;
    OnReadFinished_10(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL free-skip\n"); return 1; }

    printf("OK_004dd42f\n");
    return 0;
}