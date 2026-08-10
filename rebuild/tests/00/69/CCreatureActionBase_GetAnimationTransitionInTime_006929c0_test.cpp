#include <cstdio>

struct Foo {
    long base;
    long Method(long arg) { return base + arg; }
};
struct Sub { long seed; };

static long g_helper_ret;
static long __fastcall Sub_Helper(Sub* s) { g_helper_ret = s->seed * 3; return g_helper_ret; }

struct CCreatureActionBase {
    char pad0[8];
    Sub  sub;
    char pad1[0x74 - 8 - sizeof(Sub)];
    Foo* f74;
};

static long model(CCreatureActionBase* self)
{
    Foo* f = self->f74;
    long r = Sub_Helper(&self->sub);
    return f->Method(r);
}

int main()
{
    Foo foo; foo.base = 100;
    CCreatureActionBase o;
    o.sub.seed = 7;
    o.f74 = &foo;
    long got = model(&o);
    if (got != 100 + 21) { std::printf("FAIL got=%ld\n", got); return 1; }

    Foo foo2; foo2.base = -5;
    o.sub.seed = 0;
    o.f74 = &foo2;
    long got2 = model(&o);
    if (got2 != -5) { std::printf("FAIL got2=%ld\n", got2); return 1; }

    std::printf("OK_006929c0\n");
    return 0;
}