#include <cstdio>

float g_defaultGameAngleXY = 1.5f;

struct Obj {
    virtual void f00()=0; virtual void f01()=0; virtual void f02()=0; virtual void f03()=0;
    virtual void f04()=0; virtual void f05()=0; virtual void f06()=0; virtual void f07()=0;
    virtual void f08()=0; virtual void f09()=0; virtual void f10()=0; virtual void f11()=0;
    virtual void f12()=0; virtual void f13()=0; virtual void f14()=0; virtual void f15()=0;
    virtual void f16()=0; virtual void f17()=0; virtual void f18()=0; virtual void f19()=0;
    virtual void f20()=0; virtual void f21()=0; virtual void f22()=0; virtual void f23()=0;
    virtual void f24()=0;
    virtual float getAngle()=0;
};

struct Holder {
    virtual void g00()=0; virtual void g01()=0; virtual void g02()=0; virtual void g03()=0;
    virtual void g04()=0; virtual void g05()=0; virtual void g06()=0; virtual void g07()=0;
    virtual void g08()=0; virtual void g09()=0;
    virtual bool tryGet(Obj** out)=0;
};

struct Inner {
    char pad34[0x34];
    Holder* holder;
};

struct CGameScriptInterface {
    void* pad0;
    Inner* inner;
};

float __fastcall CGameScriptInterface_GetGameAngleXY(CGameScriptInterface* self)
{
    Obj* local;
    Holder* h = self->inner->holder;
    if (h->tryGet(&local)) {
        return local->getAngle();
    }
    return g_defaultGameAngleXY;
}

struct ObjImpl : Obj {
    void f00(){} void f01(){} void f02(){} void f03(){}
    void f04(){} void f05(){} void f06(){} void f07(){}
    void f08(){} void f09(){} void f10(){} void f11(){}
    void f12(){} void f13(){} void f14(){} void f15(){}
    void f16(){} void f17(){} void f18(){} void f19(){}
    void f20(){} void f21(){} void f22(){} void f23(){}
    void f24(){}
    float getAngle() { return 42.0f; }
};

static ObjImpl g_target;
static bool g_ret = true;

struct HolderImpl : Holder {
    void g00(){} void g01(){} void g02(){} void g03(){}
    void g04(){} void g05(){} void g06(){} void g07(){}
    void g08(){} void g09(){}
    bool tryGet(Obj** out) { *out = &g_target; return g_ret; }
};

int main() {
    static HolderImpl holder;
    static Inner inner; inner.holder = &holder;
    static CGameScriptInterface gsi; gsi.inner = &inner;

    g_ret = true;
    float a = CGameScriptInterface_GetGameAngleXY(&gsi);
    g_ret = false;
    float b = CGameScriptInterface_GetGameAngleXY(&gsi);

    if (a == 42.0f && b == 1.5f) {
        std::printf("CGameScriptInterface_006e7340_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL a=%f b=%f\n", a, b);
    return 1;
}