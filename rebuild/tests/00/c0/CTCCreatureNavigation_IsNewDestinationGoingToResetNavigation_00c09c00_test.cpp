#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void v4(){}
    virtual void v5(){}
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest)
    {
        return this->field_4->v6(this, dest);
    }
};

static CTCCreatureNavigation* g_expectNav;
static C3DVector* g_expectDest;
static bool g_ret;
bool Helper::v6(CTCCreatureNavigation* nav, C3DVector* dest)
{
    if (nav != g_expectNav) { printf("BADNAV\n"); return false; }
    if (dest != g_expectDest) { printf("BADDEST\n"); return false; }
    return g_ret;
}

int main()
{
    Helper h;
    CTCCreatureNavigation nav;
    nav.field_0 = 0;
    nav.field_4 = &h;
    C3DVector d = {1,2,3};
    g_expectNav = &nav;
    g_expectDest = &d;

    g_ret = true;
    if (nav.IsNewDestinationGoingToResetNavigation(&d) != true) { printf("FAIL1\n"); return 1; }
    g_ret = false;
    if (nav.IsNewDestinationGoingToResetNavigation(&d) != false) { printf("FAIL2\n"); return 1; }

    printf("OK_00c09c00\n");
    return 0;
}