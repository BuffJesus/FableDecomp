#include <stdio.h>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* pos) { return false; }
};

struct CTCCreatureNavigation {
    void* pad0;
    Helper* nav;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* pos)
{
    return self->nav->slot6(self, pos);
}

static CTCCreatureNavigation* g_expect_nav;
static C3DVector* g_expect_pos;
static bool g_ret;
static int g_called;

struct HelperImpl : public Helper {
    void v0() {} void v1() {} void v2() {} void v3() {} void v4() {} void v5() {}
    bool slot6(CTCCreatureNavigation* nav, C3DVector* pos) {
        g_called++;
        if (nav != g_expect_nav) { printf("FAIL nav\n"); }
        if (pos != g_expect_pos) { printf("FAIL pos\n"); }
        return g_ret;
    }
};

int main()
{
    HelperImpl h;
    CTCCreatureNavigation obj; obj.pad0 = 0; obj.nav = &h;
    C3DVector pos = {1.0f, 2.0f, 3.0f};

    g_expect_nav = &obj; g_expect_pos = &pos;

    g_ret = true; g_called = 0;
    bool r1 = IsNewDestinationGoingToResetNavigation(&obj, 0, &pos);
    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&obj, 0, &pos);

    if (r1 == true && r2 == false && g_called == 2) {
        printf("OK_00c23590\n");
        return 0;
    }
    printf("BAD r1=%d r2=%d called=%d\n", (int)r1, (int)r2, g_called);
    return 1;
}