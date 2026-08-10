#include <stdio.h>

struct CActiveFile {
    void* buf;
    int helper_called;
};

static int g_freed = 0;
static void* g_last_free = 0;

static void Helper_65f1fa(CActiveFile* self) { self->helper_called = 1; }
static void my_free(void* p) { g_freed++; g_last_free = p; }

static void OnReadFinished_65f1fa(CActiveFile* self)
{
    Helper_65f1fa(self);
    void* p = self->buf;
    if (p)
        my_free(p);
}

int main()
{
    int dummy = 0;
    CActiveFile a; a.buf = &dummy; a.helper_called = 0;
    OnReadFinished_65f1fa(&a);
    if (!a.helper_called) { printf("FAIL helper\n"); return 1; }
    if (g_freed != 1 || g_last_free != &dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b; b.buf = 0; b.helper_called = 0;
    OnReadFinished_65f1fa(&b);
    if (!b.helper_called) { printf("FAIL helper2\n"); return 1; }
    if (g_freed != 1) { printf("FAIL nofree\n"); return 1; }

    printf("OK_0065f1fa\n");
    return 0;
}