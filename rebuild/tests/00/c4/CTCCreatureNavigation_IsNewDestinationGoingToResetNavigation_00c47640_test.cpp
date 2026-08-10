#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct NavSub {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual bool IsReset(CTCCreatureNavigation* nav, C3DVector* dest) = 0;
};

struct CTCCreatureNavigation {
    void* f0;
    NavSub* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->f4->IsReset(self, dest);
}

static CTCCreatureNavigation* g_expectedNav;
static C3DVector* g_expectedDest;
static bool g_ret;

struct NavSubImpl : public NavSub {
    void v0() {} void v1() {} void v2() {} void v3() {}
    void v4() {} void v5() {} void v6() {}
    bool IsReset(CTCCreatureNavigation* nav, C3DVector* dest) {
        if (nav != g_expectedNav) { printf("bad nav\n"); return false; }
        if (dest != g_expectedDest) { printf("bad dest\n"); return false; }
        return g_ret;
    }
};

int main()
{
    NavSubImpl navSubObj;
    NavSubImpl* pNavSub = &navSubObj;
    CTCCreatureNavigation nav;
    nav.f0 = 0;
    nav.f4 = pNavSub;

    C3DVector dest = {1.0f, 2.0f, 3.0f};

    g_expectedNav = &nav;
    g_expectedDest = &dest;

    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);

    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &dest);

    if (r1 == true && r2 == false) {
        printf("OK_00c47640\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}