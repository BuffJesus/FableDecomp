#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

static void* g_owner;
static C3DVector* g_dest;
static bool g_ret;
static int g_called;

struct SubBase {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual bool v6(CTCCreatureNavigation* owner, C3DVector* dest) {
        g_called++;
        g_owner = (void*)owner;
        g_dest = dest;
        return g_ret;
    }
};

struct CTCCreatureNavigation {
    void* pad0;
    SubBase* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->f4->v6(self, dest);
}

int main() {
    SubBase node;
    CTCCreatureNavigation nav;
    nav.pad0 = 0;
    nav.f4 = &node;
    C3DVector dv = {1.0f, 2.0f, 3.0f};

    g_ret = true; g_called = 0; g_owner = 0; g_dest = 0;
    bool r = IsNewDestinationGoingToResetNavigation(&nav, 0, &dv);
    if (!r) { printf("FAIL ret true\n"); return 1; }
    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_owner != (void*)&nav) { printf("FAIL owner\n"); return 1; }
    if (g_dest != &dv) { printf("FAIL dest\n"); return 1; }

    g_ret = false;
    r = IsNewDestinationGoingToResetNavigation(&nav, 0, &dv);
    if (r) { printf("FAIL ret false\n"); return 1; }
    if (g_called != 2) { printf("FAIL called2\n"); return 1; }

    printf("OK_00c2f720\n");
    return 0;
}