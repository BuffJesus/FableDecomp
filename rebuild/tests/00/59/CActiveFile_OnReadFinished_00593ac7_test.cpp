#include <stdio.h>
#include <stdlib.h>

struct CActiveFile {
    void* m0;
};

static int g_helper_called = 0;
static void model_helper(CActiveFile* self){ g_helper_called++; }

static int g_freed = 0;
static void* g_last_free = 0;
static void model_free(void* p){ g_freed++; g_last_free = p; }

static void model_OnReadFinished(CActiveFile* self)
{
    model_helper(self);
    void* p = self->m0;
    if (p)
        model_free(p);
}

int main()
{
    int dummy = 5;
    CActiveFile a; a.m0 = &dummy;
    g_helper_called = 0; g_freed = 0; g_last_free = 0;
    model_OnReadFinished(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != 1) { printf("FAIL free\n"); return 1; }
    if (g_last_free != &dummy) { printf("FAIL freeptr\n"); return 1; }

    CActiveFile b; b.m0 = 0;
    g_helper_called = 0; g_freed = 0;
    model_OnReadFinished(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != 0) { printf("FAIL nofree\n"); return 1; }

    printf("OK_00593ac7\n");
    return 0;
}