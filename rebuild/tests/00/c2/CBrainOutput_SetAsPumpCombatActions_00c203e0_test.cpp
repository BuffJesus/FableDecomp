#include <cstdio>

struct SubObj {
    void Set(long a);
};

struct CBrainOutput {
    unsigned long flags;
    long pad;
    SubObj sub;
};

static long g_lastArg;
static void* g_lastSelf;

void SubObj::Set(long a) {
    g_lastArg = a;
    g_lastSelf = this;
}

void __fastcall SetAsPumpCombatActions(CBrainOutput* self, int /*edx*/, long a)
{
    self->sub.Set(a);
    self->flags |= 2;
}

int main()
{
    CBrainOutput o;
    o.flags = 0x1;
    o.pad = 0;
    SetAsPumpCombatActions(&o, 0, 1234);

    bool ok = (g_lastArg == 1234)
           && (g_lastSelf == (void*)&o.sub)
           && (o.flags == 0x3); // 1 | 2
    if (ok) printf("00c203e0_TEST PASS\n");
    else    printf("FAIL flags=%lx arg=%ld self=%p sub=%p\n",
                   o.flags, g_lastArg, g_lastSelf, (void*)&o.sub);
    return 0;
}