#include <cstdio>

struct CActiveFile { void* field0; };

static int g_helper_called = 0;
static void* g_freed = 0;

void __fastcall helper_4db88a(CActiveFile* self) { g_helper_called++; (void)self; }
void __cdecl free_bfea14(void* p) { g_freed = p; }

void __fastcall OnReadFinished_5(CActiveFile* self)
{
    helper_4db88a(self);
    void* p = self->field0;
    if (p)
        free_bfea14(p);
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    OnReadFinished_5(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL freed\n"); return 1; }

    g_helper_called = 0; g_freed = 0;
    CActiveFile b;
    b.field0 = 0;
    OnReadFinished_5(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != 0) { printf("FAIL nofree\n"); return 1; }

    printf("OK_004dd21c\n");
    return 0;
}