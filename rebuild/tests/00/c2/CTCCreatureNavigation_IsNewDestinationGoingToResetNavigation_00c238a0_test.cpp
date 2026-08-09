#include <stdio.h>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

static void* g_expect_self;
static void* g_expect_vec;
static bool g_ret;
static int g_calls;

struct Delegate {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual void v6() {}
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* vec) {
        g_calls++;
        if ((void*)nav != g_expect_self) { printf("FAIL nav\n"); return false; }
        if ((void*)vec != g_expect_vec) { printf("FAIL vec\n"); return false; }
        return g_ret;
    }
};

struct CTCCreatureNavigation {
    void* f0;
    Delegate* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* vec)
{
    return self->f4->v7(self, vec);
}

int main()
{
    Delegate d;
    CTCCreatureNavigation nav;
    nav.f0 = 0;
    nav.f4 = &d;
    C3DVector v = {1.0f, 2.0f, 3.0f};

    g_expect_self = &nav;
    g_expect_vec = &v;

    g_ret = true;
    if (IsNewDestinationGoingToResetNavigation(&nav, 0, &v) != true) { printf("FAIL true\n"); return 1; }
    g_ret = false;
    if (IsNewDestinationGoingToResetNavigation(&nav, 0, &v) != false) { printf("FAIL false\n"); return 1; }
    if (g_calls != 2) { printf("FAIL calls\n"); return 1; }

    printf("OK_00c238a0\n");
    return 0;
}