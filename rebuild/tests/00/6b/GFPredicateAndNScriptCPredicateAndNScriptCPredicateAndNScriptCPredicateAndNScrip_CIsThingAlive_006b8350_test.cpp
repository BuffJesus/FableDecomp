#include <cstdio>

struct CIsThingAlive { int v; };

struct Inner {
    char pad[4];
    CIsThingAlive* build(CIsThingAlive* a, CIsThingAlive* b);
};
struct Outer {
    char pad[0x90];
    Inner inner;
    CIsThingAlive* CPredicate_And(CIsThingAlive* p1, CIsThingAlive* p2);
};

static CIsThingAlive* g_a; static CIsThingAlive* g_b; static void* g_self;
CIsThingAlive* Inner::build(CIsThingAlive* a, CIsThingAlive* b) {
    g_a = a; g_b = b; g_self = this; return a;
}
CIsThingAlive* Outer::CPredicate_And(CIsThingAlive* p1, CIsThingAlive* p2) {
    inner.build(p1, p2);
    return p1;
}

int main() {
    Outer o;
    CIsThingAlive x, y;
    CIsThingAlive* r = o.CPredicate_And(&x, &y);
    bool ok = (r == &x) && (g_a == &x) && (g_b == &y)
              && (g_self == (void*)((char*)&o + 0x90));
    printf(ok ? "AND_FWD_OK\n" : "AND_FWD_FAIL\n");
    return ok ? 0 : 1;
}