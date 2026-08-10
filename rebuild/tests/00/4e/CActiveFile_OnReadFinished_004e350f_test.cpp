#include <stdio.h>
#include <stdlib.h>

struct CActiveFile { void* ptr; };

static int g_helper_calls = 0;
static void* g_freed = (void*)-1;

void helper(CActiveFile* self) { g_helper_calls++; }
void myfree(void* p) { g_freed = p; }

void model(CActiveFile* self)
{
    helper(self);
    void* p = self->ptr;
    if (p)
        myfree(p);
}

int main()
{
    CActiveFile a;
    int dummy = 5;
    a.ptr = &dummy;
    g_helper_calls = 0; g_freed = (void*)-1;
    model(&a);
    if (g_helper_calls != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free nonnull\n"); return 1; }

    CActiveFile b;
    b.ptr = 0;
    g_helper_calls = 0; g_freed = (void*)-1;
    model(&b);
    if (g_helper_calls != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != (void*)-1) { printf("FAIL free null\n"); return 1; }

    printf("OK_004e350f\n");
    return 0;
}