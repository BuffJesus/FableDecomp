#include <cstdio>

struct CActiveFile { CActiveFile* f0; };

static int g_helper_called = 0;
static void* g_freed = 0;

static void helper(CActiveFile* self) { g_helper_called++; (void)self; }
static void freefn(void* p) { g_freed = p; }

static void model(CActiveFile* self)
{
    helper(self);
    CActiveFile* p = self->f0;
    if (p)
        freefn(p);
}

int main()
{
    CActiveFile child; child.f0 = 0;
    CActiveFile a; a.f0 = &child;
    model(&a);
    int ok1 = (g_helper_called == 1) && (g_freed == (void*)&child);

    g_helper_called = 0; g_freed = 0;
    CActiveFile b; b.f0 = 0;
    model(&b);
    int ok2 = (g_helper_called == 1) && (g_freed == 0);

    if (ok1 && ok2) printf("OK_004dd470\n");
    else printf("FAIL\n");
    return (ok1 && ok2) ? 0 : 1;
}