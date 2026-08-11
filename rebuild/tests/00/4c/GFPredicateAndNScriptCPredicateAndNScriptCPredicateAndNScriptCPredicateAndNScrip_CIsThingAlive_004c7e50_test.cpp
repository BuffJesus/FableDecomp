#include <cstdio>

struct CIsThingAlive;
struct CPredicate_And;

struct Callee {
    void F(CIsThingAlive* p, CPredicate_And* self);
};

struct GlobHolder {
    void*   m0;
    Callee* m4;
};

GlobHolder* g_holder;

static int g_calls = 0;
static void* g_seen_p = 0;
static void* g_seen_self = 0;

void Callee::F(CIsThingAlive* p, CPredicate_And* self)
{
    g_calls++;
    g_seen_p = (void*)p;
    g_seen_self = (void*)self;
}

CIsThingAlive* __fastcall Ctor_CPredicate_And_NScript(CPredicate_And* self, int edx, CIsThingAlive* p)
{
    g_holder->m4->F(p, self);
    return p;
}

int main()
{
    Callee callee;
    GlobHolder h;
    h.m0 = 0;
    h.m4 = &callee;
    g_holder = &h;

    CPredicate_And* self = (CPredicate_And*)0x11110000;
    CIsThingAlive*  p    = (CIsThingAlive*)0x22220000;

    CIsThingAlive* r = Ctor_CPredicate_And_NScript(self, 0, p);

    bool ok = (r == p)
           && (g_calls == 1)
           && (g_seen_p == (void*)p)
           && (g_seen_self == (void*)self);

    printf("r=%p calls=%d\n", (void*)r, g_calls);
    if (ok) { printf("PARITY_OK_004c7e50\n"); return 0; }
    printf("FAIL\n");
    return 1;
}