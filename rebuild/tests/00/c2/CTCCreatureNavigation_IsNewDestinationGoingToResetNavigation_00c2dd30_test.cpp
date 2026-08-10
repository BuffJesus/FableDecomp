#include <stdio.h>

struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

struct Inner {
    virtual void v0()=0;
    virtual void v1()=0;
    virtual void v2()=0;
    virtual void v3()=0;
    virtual void v4()=0;
    virtual void v5()=0;
    virtual bool v6(CTCCreatureNavigation* a, C3DVector* b)=0;
};

struct CTCCreatureNavigation {
    void* field_0;
    Inner* field_4;
};

static CTCCreatureNavigation* g_seen_this;
static C3DVector* g_seen_dest;
static Inner* g_seen_ecx;
static bool g_ret;

struct InnerImpl : Inner {
    void v0() {} void v1() {} void v2() {} void v3() {} void v4() {} void v5() {}
    bool v6(CTCCreatureNavigation* a, C3DVector* b) {
        g_seen_ecx = this;
        g_seen_this = a;
        g_seen_dest = b;
        return g_ret;
    }
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest)
{
    return self->field_4->v6(self, dest);
}

int main()
{
    InnerImpl inner;
    CTCCreatureNavigation nav;
    nav.field_0 = 0;
    nav.field_4 = &inner;

    C3DVector d = {1,2,3};

    g_ret = true;
    bool r = IsNewDestinationGoingToResetNavigation(&nav, 0, &d);
    if (r != true) { printf("FAIL ret\n"); return 1; }
    if (g_seen_ecx != &inner) { printf("FAIL ecx\n"); return 1; }
    if (g_seen_this != &nav) { printf("FAIL this\n"); return 1; }
    if (g_seen_dest != &d) { printf("FAIL dest\n"); return 1; }

    g_ret = false;
    r = IsNewDestinationGoingToResetNavigation(&nav, 0, &d);
    if (r != false) { printf("FAIL ret2\n"); return 1; }

    printf("OK_00c2dd30\n");
    return 0;
}