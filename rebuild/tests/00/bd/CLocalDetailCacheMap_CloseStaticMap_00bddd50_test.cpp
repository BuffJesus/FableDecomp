#include <cstdio>

struct SubObj {
    void Method(int arg);
};

struct CLocalDetailCacheMap {
    int    f0;
    int    f4;
    int    f8;
    SubObj sub;
};

static SubObj* g_seen_self = 0;
static int     g_seen_arg  = -999;

void SubObj::Method(int arg)
{
    g_seen_self = this;
    g_seen_arg  = arg;
}

void __fastcall CloseStaticMap(CLocalDetailCacheMap* self)
{
    self->sub.Method(self->f4);
    self->f4 = 0;
}

int main()
{
    CLocalDetailCacheMap o;
    o.f0 = 111; o.f4 = 42; o.f8 = 333;

    CloseStaticMap(&o);

    bool ok = (g_seen_arg == 42)
           && (g_seen_self == &o.sub)
           && (o.f4 == 0)
           && (o.f0 == 111) && (o.f8 == 333);

    if (ok) printf("00bddd50_TEST PASS\n");
    else    printf("FAIL arg=%d f4=%d\n", g_seen_arg, o.f4);
    return 0;
}