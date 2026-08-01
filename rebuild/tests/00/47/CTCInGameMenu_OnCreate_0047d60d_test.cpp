#include <cstdio>

// ---- Mock counted-pointer object the getter hands back --------------------
// layout: [0]=vtable, [4]=refcount, [0xa8]=field consumed by the member call
struct MockCounted
{
    void** vtbl;
    int    refcount;
    char   pad[0xa8 - 8];
    int    fieldA8;
};

struct MockSub  { char pad[0x70]; void* getterCookie; };
struct MockThis { void* v0; MockSub* sub4; };

static MockCounted g_counted;
static int   g_applied_field = 0;
static void* g_apply_this    = 0;
static void* g_getter_ecx    = 0;
static int   g_dtor_called   = 0;

// __fastcall with (ecx arg, unused edx, one STACK arg) => cleans 4 bytes on
// return (ret 4), exactly matching what the source pushed before the call.
extern "C" void __fastcall GetterImpl(void* ecxCookie, int /*edx*/, MockCounted** outLocal)
{
    g_getter_ecx = ecxCookie;
    *outLocal = &g_counted;
}
extern "C" void __fastcall ApplyImpl(void* ecxThis, int /*edx*/, int field)
{
    g_apply_this    = ecxThis;
    g_applied_field = field;
}
extern "C" void __fastcall DtorImpl(void* ecxThis)
{
    (void)ecxThis;
    g_dtor_called = 1;
}

// Masked callees, named exactly as the source references them. The source set
// ecx and pushed one stack arg before each call; forward straight to the
// __fastcall impls (ecx = arg1 already; the pushed arg is the stack param, so
// the impl's ret 4 balances the push).
extern "C" __declspec(naked) void GetInGameMenuCountedPtr_0047d60d(void)
{
    __asm { jmp GetterImpl }
}
extern "C" __declspec(naked) void ApplyInGameMenuField_0047d60d(void)
{
    __asm { jmp ApplyImpl }
}

extern "C" void __fastcall CTCInGameMenu_OnCreate_0047d60d(void* self);

int main()
{
    static void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&DtorImpl;

    g_counted.vtbl     = vt;
    g_counted.refcount = 2;          // dec -> 1 (nonzero): dtor must NOT fire
    g_counted.fieldA8  = 0x0BADF00D;

    void* cookie = (void*)0xC0FFEE;
    static MockSub subObj;
    subObj.getterCookie = cookie;
    MockThis obj;
    obj.v0   = 0;
    obj.sub4 = &subObj;

    CTCInGameMenu_OnCreate_0047d60d(&obj);

    int ok = 1;
    ok &= (g_getter_ecx    == cookie);
    ok &= (g_apply_this    == (void*)&obj);
    ok &= (g_applied_field == 0x0BADF00D);
    ok &= (g_counted.refcount == 1);
    ok &= (g_dtor_called   == 0);

    if (ok) { std::printf("INGAMEMENU_ONCREATE_COUNTEDPTR_RELEASE_TEST PASS\n"); return 0; }
    std::printf("INGAMEMENU_ONCREATE_COUNTEDPTR_RELEASE_TEST FAIL\n");
    return 1;
}