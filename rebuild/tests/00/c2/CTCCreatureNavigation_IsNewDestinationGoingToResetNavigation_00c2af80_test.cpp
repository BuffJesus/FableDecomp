#include <cstdio>

struct C3DVector { float x, y, z; };

static void* g_this;
static C3DVector* g_p;
static bool g_ret;

struct Sub {
    virtual bool m00() { return false; }
    virtual bool m04() { return false; }
    virtual bool m08() { return false; }
    virtual bool m0c() { return false; }
    virtual bool m10() { return false; }
    virtual bool m14() { return false; }
    virtual bool Check(void* origThis, C3DVector* p) { g_this=origThis; g_p=p; return g_ret; }
};

struct CTCCreatureNavigation {
    void* f0;
    Sub*  f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* p)
{
    return self->f4->Check(self, p);
}

int main()
{
    Sub navSub;
    CTCCreatureNavigation nav; nav.f0 = 0; nav.f4 = &navSub;
    C3DVector v = {1.f, 2.f, 3.f};

    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);
    if (!r1) { printf("FAIL r1\n"); return 1; }
    if (g_this != &nav) { printf("FAIL this\n"); return 1; }
    if (g_p != &v) { printf("FAIL p\n"); return 1; }

    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);
    if (r2) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c2af80\n");
    return 0;
}