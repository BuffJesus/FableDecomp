#include <cstdio>

struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

static void* g_seen_this;
static C3DVector* g_seen_dest;
static bool g_ret;

struct SubObj {
    virtual void s0() {}
    virtual void s1() {}
    virtual void s2() {}
    virtual void s3() {}
    virtual void s4() {}
    virtual void s5() {}
    virtual void s6() {}
    virtual bool s7(CTCCreatureNavigation* thisNav, C3DVector* dest) {
        g_seen_this = thisNav;
        g_seen_dest = dest;
        return g_ret;
    }
};

struct CTCCreatureNavigation {
    void* pad0;
    SubObj* subptr;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest)
{
    return self->subptr->s7(self, dest);
}

int main()
{
    SubObj so;
    SubObj* pso = &so;
    CTCCreatureNavigation nav;
    CTCCreatureNavigation* pnav = &nav;
    nav.pad0 = 0;
    nav.subptr = pso;
    C3DVector vec;
    C3DVector* pvec = &vec;
    vec.x = 1; vec.y = 2; vec.z = 3;

    g_ret = true;
    bool r = IsNewDestinationGoingToResetNavigation(pnav, 0, pvec);
    if (!r) { printf("FAIL true\n"); return 1; }
    if (g_seen_this != pnav) { printf("FAIL this\n"); return 1; }
    if (g_seen_dest != pvec) { printf("FAIL dest\n"); return 1; }

    g_ret = false;
    r = IsNewDestinationGoingToResetNavigation(pnav, 0, pvec);
    if (r) { printf("FAIL false\n"); return 1; }

    printf("OK_00c2f4e0\n");
    return 0;
}