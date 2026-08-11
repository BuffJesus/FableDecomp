#include <cstdio>

struct CThingTrackNode;

struct Obj {
    virtual void s00()=0; virtual void s01()=0; virtual void s02()=0; virtual void s03()=0;
    virtual void s04()=0; virtual void s05()=0; virtual void s06()=0; virtual void s07()=0;
    virtual void s08()=0; virtual void s09()=0; virtual void s10()=0; virtual void s11()=0;
    virtual void s12()=0; virtual void s13()=0; virtual void s14()=0; virtual void s15()=0;
    virtual void s16()=0; virtual void s17()=0;
    virtual void s18(CThingTrackNode* arg)=0;
};

static int g_called = 0;
static Obj* g_this = 0;
static CThingTrackNode* g_arg = 0;

struct ObjImpl : public Obj {
    void s00() {} void s01() {} void s02() {} void s03() {}
    void s04() {} void s05() {} void s06() {} void s07() {}
    void s08() {} void s09() {} void s10() {} void s11() {}
    void s12() {} void s13() {} void s14() {} void s15() {}
    void s16() {} void s17() {}
    void s18(CThingTrackNode* arg) { g_called++; g_this = this; g_arg = arg; }
};

static ObjImpl g_obj;

extern "C" Obj* __fastcall CThingTrackNode_GetObj(CThingTrackNode* self)
{
    (void)self;
    return &g_obj;
}

Obj* __fastcall DrawIsDrawable(CThingTrackNode* self);

int main()
{
    CThingTrackNode* fake = (CThingTrackNode*)0x12345678;
    Obj* r = DrawIsDrawable(fake);

    bool ok = (r == (Obj*)&g_obj) && (g_called == 1) &&
              (g_this == (Obj*)&g_obj) && (g_arg == fake);
    if (ok) printf("00b05ab0_TEST PASS\n");
    else printf("FAIL r=%p called=%d this=%p arg=%p\n",
                (void*)r, g_called, (void*)g_this, (void*)g_arg);
    return 0;
}