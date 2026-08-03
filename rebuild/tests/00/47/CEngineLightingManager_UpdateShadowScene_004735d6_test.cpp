// Behavioral fixture for CEngineLightingManager::UpdateShadowScene @ 0x004735D6.
//
// Compiles /Od, links, runs, prints an exact uppercase pass token.
// Provides faithful definitions for the masked callees (Profile_Enter and the
// two out-of-line __fastcall members) so the whole thing links standalone.
//
// Mode dispatch on this->mode (+0x18):
//   case 3 -> p28->Push(*(void**)arg)   (single stack arg = *arg)
//   case 2 -> guard.Apply(p24, arg)     (guard is local, this = &guard)
//   other  -> guard call only, no dispatch

#include <cstdio>
#include <cstring>

extern "C" void __stdcall Profile_Enter(int marker);

struct ShadowGuard {
    void* slot;
    void Apply(void* a, void* b);
};

struct ShadowSink {
    void Push(void* v);
};

struct CEngineLightingManager {
    char   pad0[0x18];
    int    mode;              /* +0x18 */
    char   pad1[0x24 - 0x1c];
    void*  p24;               /* +0x24 */
    ShadowSink* p28;          /* +0x28 */

    void UpdateShadowScene(void* arg);
};

static int   g_profileCalls;
static int   g_profileMarker;

static int   g_applyCalls;
static void* g_applyThis;
static void* g_applyA;
static void* g_applyB;

static int   g_pushCalls;
static void* g_pushThis;
static void* g_pushVal;

extern "C" void __stdcall Profile_Enter(int marker)
{
    ++g_profileCalls;
    g_profileMarker = marker;
}

void ShadowGuard::Apply(void* a, void* b)
{
    ++g_applyCalls;
    g_applyThis = this;
    g_applyA = a;
    g_applyB = b;
}

void ShadowSink::Push(void* v)
{
    ++g_pushCalls;
    g_pushThis = this;
    g_pushVal = v;
}

// Recreated body (identical spelling to the reconstruction) so the fixture is
// self-contained and does not need the parity object.
void CEngineLightingManager::UpdateShadowScene(void* arg)
{
    ShadowGuard guard;
    Profile_Enter(0x122d70e);
    guard.slot = 0;
    switch (this->mode) {
    case 2:
        guard.Apply(this->p24, arg);
        break;
    case 3:
        this->p28->Push(*(void**)arg);
        break;
    }
}

static void Reset()
{
    g_profileCalls = 0; g_profileMarker = 0;
    g_applyCalls = 0; g_applyThis = 0; g_applyA = 0; g_applyB = 0;
    g_pushCalls = 0; g_pushThis = 0; g_pushVal = 0;
}

int main()
{
    // Layout assertions must hold for the offsets the reconstruction assumes.
    if (((char*)&((CEngineLightingManager*)0)->mode - (char*)0) != 0x18) return 90;
    if (((char*)&((CEngineLightingManager*)0)->p24 - (char*)0) != 0x24) return 91;
    if (((char*)&((CEngineLightingManager*)0)->p28 - (char*)0) != 0x28) return 92;

    CEngineLightingManager mgr;

    // ---- Mode 3: p28->Push(*arg) ----
    Reset();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 3;
    ShadowSink sink;
    mgr.p28 = &sink;
    void* inner = (void*)0xbeef5678;
    void* cell = &inner;            // arg points at a slot holding `inner`
    mgr.UpdateShadowScene(&cell);   // *(void**)arg == cell
    if (g_profileCalls != 1 || g_profileMarker != 0x122d70e) return 1;
    if (g_pushCalls != 1) return 2;
    if (g_pushThis != &sink) return 3;
    if (g_pushVal != cell) return 4;   // pushed value = *(void**)arg
    if (g_applyCalls != 0) return 5;

    // ---- Mode 2: guard.Apply(p24, arg) ----
    Reset();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 2;
    void* field = (void*)0x11112222;
    mgr.p24 = field;
    void* arg2 = (void*)0x33334444;
    mgr.UpdateShadowScene(arg2);
    if (g_profileCalls != 1 || g_profileMarker != 0x122d70e) return 6;
    if (g_applyCalls != 1) return 7;
    if (g_applyA != field) return 8;   // first arg = this->p24
    if (g_applyB != arg2) return 9;    // second arg = the argument
    if (g_applyThis == 0) return 15;   // local-this is non-null
    if (g_pushCalls != 0) return 10;

    // ---- Other mode: guard call only. ----
    Reset();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 5;
    mgr.UpdateShadowScene((void*)0x55556666);
    if (g_profileCalls != 1) return 11;
    if (g_applyCalls != 0 || g_pushCalls != 0) return 12;

    // ---- Mode 1 (the dec/dec/dec == -1 fallthrough) also does nothing. ----
    Reset();
    std::memset(&mgr, 0, sizeof(mgr));
    mgr.mode = 1;
    mgr.UpdateShadowScene((void*)0x77778888);
    if (g_profileCalls != 1) return 13;
    if (g_applyCalls != 0 || g_pushCalls != 0) return 14;

    std::printf("UPDATE_SHADOW_SCENE_004735D6_TEST PASS\n");
    return 0;
}