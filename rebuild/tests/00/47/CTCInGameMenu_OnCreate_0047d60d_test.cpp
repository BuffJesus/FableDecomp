#include <cstdio>

struct GuiObject {
    virtual void v0();
    virtual void v1();
    int          refs;          // +0x4
    char         pad[0xa8 - 8];
    int          fieldA8;       // +0xa8
};

struct GuiHolder { GuiObject* obj; GuiHolder() : obj(0) {} };
struct GuiProvider { void GetHeld(GuiHolder* out); };
struct Sub4 { char pad[0x70]; GuiProvider* provider70; };
struct CTCInGameMenu {
    void* v0field;
    Sub4* field4;
    void ApplyGuiField(int v);
    void OnCreate();
};

// Base virtuals need bodies so the vtable emits; the impl below overrides them.
void GuiObject::v0() {}
void GuiObject::v1() {}

// ---- observation state ----
static void* g_getter_this = 0;
static void* g_apply_this  = 0;
static int   g_applied     = 0;
static int   g_v1_called   = 0;

// The object the getter returns; its v1 virtual (vtbl slot 1) sets a flag.
struct GuiObjectImpl : GuiObject {
    virtual void v0() {}
    virtual void v1() { g_v1_called = 1; }
};
static GuiObjectImpl g_impl;

// masked callee 1: fills the counted-pointer local (ecx = provider70)
void GuiProvider::GetHeld(GuiHolder* out)
{
    g_getter_this = (void*)this;
    out->obj = &g_impl;
}

// masked callee 2: consumes object->fieldA8 (ecx = the menu this)
void CTCInGameMenu::ApplyGuiField(int v)
{
    g_apply_this = (void*)this;
    g_applied    = v;
}

// ---- function under test (identical body to source_cpp) ----
void CTCInGameMenu::OnCreate()
{
    GuiHolder h;
    field4->provider70->GetHeld(&h);
    GuiObject* o = h.obj;
    ApplyGuiField(o->fieldA8);
    if (--o->refs == 0)
        o->v1();
}

int main()
{
    g_impl.refs    = 2;             // dec -> 1 (nonzero): v1 must NOT fire
    g_impl.fieldA8 = 0x0BADF00D;

    static GuiProvider prov;
    static Sub4 subInst;
    GuiProvider* provPtr = &prov;
    Sub4* subPtr = &subInst;
    subInst.provider70 = provPtr;

    CTCInGameMenu menu;
    menu.v0field = 0;
    menu.field4  = subPtr;

    menu.OnCreate();

    int ok = 1;
    ok &= (g_getter_this == (void*)provPtr);
    ok &= (g_apply_this  == (void*)&menu);
    ok &= (g_applied     == 0x0BADF00D);
    ok &= (g_impl.refs   == 1);
    ok &= (g_v1_called   == 0);

    if (ok) { std::printf("INGAMEMENU_ONCREATE_COUNTEDPTR_RELEASE_TEST PASS\n"); return 0; }
    std::printf("INGAMEMENU_ONCREATE_COUNTEDPTR_RELEASE_TEST FAIL\n");
    return 1;
}