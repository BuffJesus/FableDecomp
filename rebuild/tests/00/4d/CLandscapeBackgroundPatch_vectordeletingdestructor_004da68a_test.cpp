#include <cstdio>

static int g_dtor_calls = 0;
static void* g_freed = 0;

struct Sub { int x; };
struct Obj { int pad0; Sub sub; };

static void Sub_dtor(Sub* s) { (void)s; g_dtor_calls++; }
static void eng_delete(void* p) { g_freed = p; }

static void* vdtor(Obj* self, unsigned int flags)
{
    Sub_dtor(&self->sub);
    if (flags & 1) eng_delete(self);
    return self;
}

int main()
{
    Obj a;
    void* r1 = vdtor(&a, 0);
    bool ok1 = (r1 == &a) && (g_dtor_calls == 1) && (g_freed == 0);
    void* r2 = vdtor(&a, 1);
    bool ok2 = (r2 == &a) && (g_dtor_calls == 2) && (g_freed == &a);
    g_freed = 0;
    void* r3 = vdtor(&a, 2);
    bool ok3 = (r3 == &a) && (g_dtor_calls == 3) && (g_freed == 0);
    if (ok1 && ok2 && ok3) printf("VDTOR_OK\n");
    else printf("VDTOR_FAIL\n");
    return 0;
}