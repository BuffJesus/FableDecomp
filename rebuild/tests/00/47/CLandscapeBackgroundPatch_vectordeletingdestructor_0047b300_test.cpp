#include <cstdio>

struct CLandscapeBackgroundPatch { int m_vtbl; void* member; };

static int g_innerCalls = 0;
static int g_deleteCalls = 0;
static void* g_lastInnerArg = 0;
static void* g_lastDeleteArg = 0;

static void InnerDtor(void* subAtPlus4) { g_innerCalls++; g_lastInnerArg = subAtPlus4; }
static void EngineOperatorDelete(void* p) { g_deleteCalls++; g_lastDeleteArg = p; }

static void* VecDelDtor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    InnerDtor((char*)self + 4);
    if (flags & 1)
        EngineOperatorDelete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.m_vtbl = 0; obj.member = 0;

    void* r1 = VecDelDtor(&obj, 0, 0);
    void* r2 = VecDelDtor(&obj, 0, 1);
    void* r3 = VecDelDtor(&obj, 0, 2);

    bool ok = true;
    if (r1 != &obj || r2 != &obj || r3 != &obj) ok = false;
    if (g_innerCalls != 3) ok = false;
    if (g_deleteCalls != 1) ok = false;
    if (g_lastInnerArg != (void*)((char*)&obj + 4)) ok = false;
    if (g_lastDeleteArg != &obj) ok = false;

    if (ok) printf("VECDTOR_OK\n");
    else printf("VECDTOR_FAIL inner=%d del=%d\n", g_innerCalls, g_deleteCalls);
    return 0;
}