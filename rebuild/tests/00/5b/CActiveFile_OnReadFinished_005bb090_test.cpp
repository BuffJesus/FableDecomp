#include <stdio.h>
#include <stdlib.h>
static int g_helper_called = 0;
static int g_freed = 0;
struct CActiveFile { void* buf; };
static void helper(CActiveFile* self){ g_helper_called++; }
static void OnReadFinished_4_model(CActiveFile* self)
{
    helper(self);
    void* p = self->buf;
    if (p) { free(p); g_freed++; }
}
int main()
{
    CActiveFile a; a.buf = malloc(16);
    g_helper_called = g_freed = 0;
    OnReadFinished_4_model(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_freed != 1) { printf("FAIL free\n"); return 1; }
    CActiveFile b; b.buf = 0;
    g_helper_called = g_freed = 0;
    OnReadFinished_4_model(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != 0) { printf("FAIL free2\n"); return 1; }
    printf("OK_005bb090\n");
    return 0;
}