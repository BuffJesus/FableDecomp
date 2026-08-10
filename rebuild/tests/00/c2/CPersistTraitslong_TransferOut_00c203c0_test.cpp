#include <cstdio>

struct CPersistContext;

struct SubObj {
    int dummy;
    void Method(CPersistContext* ctx);
};

struct CPersistTraitsLong {
    int flags;
    char pad[4];
    SubObj sub;
};

static SubObj* g_lastSub = 0;
static void* g_lastCtx = (void*)0;

void SubObj::Method(CPersistContext* ctx)
{
    g_lastSub = this;
    g_lastCtx = (void*)ctx;
}

void __fastcall TransferOut(CPersistTraitsLong* self, int /*edx*/, CPersistContext* ctx);

int main()
{
    CPersistTraitsLong obj;
    obj.flags = 0;
    obj.sub.dummy = 42;
    void* ctx = (void*)0xDEADBEEF;

    TransferOut(&obj, 0, (CPersistContext*)ctx);

    bool ok = true;
    if (g_lastSub != &obj.sub) ok = false;
    if (g_lastCtx != ctx) ok = false;
    if (obj.flags != 1) ok = false;

    if (ok) printf("00c203c0_TEST PASS\n");
    else printf("FAIL sub=%p ctx=%p flags=%d\n", (void*)g_lastSub, g_lastCtx, obj.flags);
    return 0;
}