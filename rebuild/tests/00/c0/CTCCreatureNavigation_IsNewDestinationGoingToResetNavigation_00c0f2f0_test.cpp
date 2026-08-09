#include <cstdio>

struct C3DVector { float x,y,z; };

struct CTCCreatureNavigation;

struct Helper {
    virtual void  s0() {}
    virtual void  s1() {}
    virtual void  s2() {}
    virtual void  s3() {}
    virtual void  s4() {}
    virtual void  s5() {}
    virtual bool  slot6(CTCCreatureNavigation* outerThis, C3DVector* vec);
};

struct CTCCreatureNavigation {
    void*   pad0;
    Helper* helper;   // +4
};

static CTCCreatureNavigation* g_expectedOuter = 0;
static C3DVector* g_expectedVec = 0;
static bool g_ret = false;
static int  g_calls = 0;

bool Helper::slot6(CTCCreatureNavigation* outerThis, C3DVector* vec)
{
    ++g_calls;
    if (outerThis != g_expectedOuter) std::printf("BAD outer\n");
    if (vec != g_expectedVec) std::printf("BAD vec\n");
    return g_ret;
}

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest)
{
    return self->helper->slot6(self, dest);
}

int main()
{
    Helper h;
    CTCCreatureNavigation nav;
    nav.pad0 = 0;
    nav.helper = &h;
    C3DVector v = {1.0f,2.0f,3.0f};

    g_expectedOuter = &nav;
    g_expectedVec = &v;

    g_ret = true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);

    g_ret = false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &v);

    if (r1 == true && r2 == false && g_calls == 2) {
        std::printf("OK_00c0f2f0\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}