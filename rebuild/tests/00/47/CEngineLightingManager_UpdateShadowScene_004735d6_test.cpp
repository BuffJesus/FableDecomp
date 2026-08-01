// Real-object ABI/behavior fixture for
// CEngineLightingManager::UpdateShadowScene @ 0x004735D6.
//
// Exercises the mode dispatch on this+0x18: mode 3 forwards *arg through the
// this+0x28 helper; mode 2 forwards (this+0x24, arg) through the local-this
// helper; any other mode does nothing but the leading guard call.
//
// Links via the source+test fallback: source.obj defines the naked target and
// references these masked __fastcall callees; this test defines faithful stubs
// for them (and the guard cookie) with NO duplicate target definition.
//
// Mode-2 ABI note: the retail body does `push arg; push [esi+0x24]; call`, so
// under __fastcall the FIRST stack argument (a3) is this+0x24 (field24) and the
// SECOND (a4) is arg -- the two stack slots are in push (not declaration) order.

#include <cstdio>
#include <cstring>

struct LightingManager
{
    unsigned char pad0[0x18];
    int mode;          // +0x18
    unsigned char pad1[0x24 - 0x1c];
    void* field24;     // +0x24
    void* sub28;       // +0x28
};

extern "C" void __fastcall
CEngineLightingManager_UpdateShadowScene_004735d6(void* self, void*, void* arg);

extern "C" const int g_FableShadowSceneGuardCookie = 0x122d70e;

static int g_guardCalls;
static void* g_guardSelf;

static int g_mode3Calls;
static void* g_mode3Obj;
static void* g_mode3Value;

static int g_mode2Calls;
static void* g_mode2Local;
static void* g_mode2Field;
static void* g_mode2Arg;

extern "C" void __fastcall FableShadowSceneGuard(void* self, void*)
{
    ++g_guardCalls;
    g_guardSelf = self;
}

extern "C" void __fastcall FableShadowSceneMode3(void* obj, void*, void* value)
{
    ++g_mode3Calls;
    g_mode3Obj = obj;
    g_mode3Value = value;
}

// __fastcall: ecx=local, edx unused; first stack arg (field) = this+0x24,
// second stack arg (arg) = the shadow-scene argument.
extern "C" void __fastcall
FableShadowSceneMode2(void* local, void*, void* field, void* arg)
{
    ++g_mode2Calls;
    g_mode2Local = local;
    g_mode2Field = field;
    g_mode2Arg = arg;
}

static void ResetCounters()
{
    g_guardCalls = 0;
    g_guardSelf = 0;
    g_mode3Calls = 0;
    g_mode3Obj = 0;
    g_mode3Value = 0;
    g_mode2Calls = 0;
    g_mode2Local = 0;
    g_mode2Field = 0;
    g_mode2Arg = 0;
}

int main()
{
    // Offsets must match the reconstruction's layout assumptions.
    if (((char*)&((LightingManager*)0)->mode - (char*)0) != 0x18) return 90;
    if (((char*)&((LightingManager*)0)->field24 - (char*)0) != 0x24) return 91;
    if (((char*)&((LightingManager*)0)->sub28 - (char*)0) != 0x28) return 92;

    LightingManager mgr;

    // ---- Mode 3: forwards *arg to the +0x28 sub-object helper. ----
    ResetCounters();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 3;
    void* sub = (void*)0xdead1234;
    mgr.sub28 = sub;
    void* argValue = (void*)0xbeef5678;
    void* argCell = &argValue;
    CEngineLightingManager_UpdateShadowScene_004735d6(&mgr, 0, &argCell);
    if (g_guardCalls != 1 || g_guardSelf != &mgr) return 1;
    if (g_mode3Calls != 1) return 2;
    if (g_mode3Obj != sub) return 3;
    if (g_mode3Value != argCell) return 4; // pushed *arg == contents of argCell slot
    if (g_mode2Calls != 0) return 5;

    // ---- Mode 2: forwards (this+0x24 as field, arg) via local-this helper. ----
    ResetCounters();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 2;
    void* field = (void*)0x11112222;
    mgr.field24 = field;
    void* arg2 = (void*)0x33334444;
    CEngineLightingManager_UpdateShadowScene_004735d6(&mgr, 0, arg2);
    if (g_guardCalls != 1 || g_guardSelf != &mgr) return 6;
    if (g_mode2Calls != 1) return 7;
    if (g_mode2Field != field) return 8; // first stack arg = this+0x24
    if (g_mode2Arg != arg2) return 9;     // second stack arg = the argument
    if (g_mode2Local == 0) return 15;     // local-this is &[ebp-4], non-null
    if (g_mode3Calls != 0) return 10;

    // ---- Other mode: guard only, no dispatch. ----
    ResetCounters();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 5;
    CEngineLightingManager_UpdateShadowScene_004735d6(&mgr, 0, (void*)0x55556666);
    if (g_guardCalls != 1 || g_guardSelf != &mgr) return 11;
    if (g_mode2Calls != 0 || g_mode3Calls != 0) return 12;

    // ---- Mode 1 (mode-2 == -1 path): also falls through. ----
    ResetCounters();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 1;
    CEngineLightingManager_UpdateShadowScene_004735d6(&mgr, 0, (void*)0x77778888);
    if (g_guardCalls != 1) return 13;
    if (g_mode2Calls != 0 || g_mode3Calls != 0) return 14;

    std::printf("UPDATE_SHADOW_SCENE_004735D6_TEST PASS\n");
    return 0;
}