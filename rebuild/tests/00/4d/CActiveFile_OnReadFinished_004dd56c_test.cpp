#include <stdio.h>

struct CActiveFile { void* field0; };

static int g_helper_calls = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_helper(CActiveFile* self) { (void)self; g_helper_calls++; }
void __cdecl free_thing(void* p) { g_freed = p; }

void __fastcall OnReadFinished_13(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->field0;
    if (p)
        free_thing(p);
}

int main()
{
    CActiveFile a;
    int dummy = 0;
    a.field0 = &dummy;
    g_helper_calls = 0; g_freed = (void*)0;
    OnReadFinished_13(&a);
    if (g_helper_calls != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL freed\n"); return 1; }

    CActiveFile b;
    b.field0 = (void*)0;
    g_helper_calls = 0; g_freed = (void*)1;
    OnReadFinished_13(&b);
    if (g_helper_calls != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL freed2\n"); return 1; }

    printf("OK_004dd56c\n");
    return 0;
}