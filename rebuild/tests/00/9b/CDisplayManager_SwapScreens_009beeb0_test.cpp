#include <new>
// Fixture: CDisplayManager::SwapScreens (0x009BEEB0).
#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

struct FakeDev { void** vt; };
static void* g_vt[64];
static int g_seq, g_presentAt, g_postAt; static HRESULT g_result; static void* g_self; static const void* g_args[4];
static HRESULT __stdcall FakePresent(void* self, const void* a, const void* b, void* c, const void* d)
{
    g_presentAt = ++g_seq; g_self = self; g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d;
    return g_result;
}
static CVsyncCallbackTimer* g_postThis;
void CVsyncCallbackTimer::OnPostSwapScreens() { g_postAt = ++g_seq; g_postThis = this; }

static unsigned char g_storage[sizeof(CDisplayManager) + 16];
static unsigned char g_timer[64];

static void Run(HRESULT result, bool expectOk, long screen, long backBuffers, long expectScreen)
{
    CDisplayManager* dm = reinterpret_cast<CDisplayManager*>(g_storage);
    memset(g_storage, 0xCD, sizeof(g_storage));
    static FakeDev dev = { g_vt };
    dm->D3DDevice = reinterpret_cast<IDirect3DDevice9*>(&dev);
    new (&dm->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>(reinterpret_cast<CVsyncCallbackTimer*>(reinterpret_cast<CVsyncCallbackTimer*>(g_timer)));
    dm->FrameCounter = 41; dm->DeviceLostFlag = false;
    dm->CurrentScreenIndex = screen; dm->NoBackBuffers = backBuffers;
    g_seq = g_presentAt = g_postAt = 0; g_postThis = 0; g_result = result;
    const bool ok = dm->SwapScreens();
    EXPECT(ok == expectOk);
    EXPECT(dm->FrameCounter == 42);
    EXPECT(g_presentAt == 1 && g_self == &dev);
    EXPECT(!g_args[0] && !g_args[1] && !g_args[2] && !g_args[3]);
    EXPECT(dm->DeviceLostFlag == (result == D3DERR_DEVICELOST));
    if (expectOk) { EXPECT(g_postAt == 2 && g_postThis == (void*)g_timer); }
    else { EXPECT(g_postAt == 0); }
    EXPECT(dm->CurrentScreenIndex == expectScreen);
    EXPECT(dm->NoBackBuffers == backBuffers);
    EXPECT(g_storage[sizeof(CDisplayManager)] == 0xCD);
}

int main()
{
    g_vt[17] = (void*)&FakePresent;
    Run(0, true, 0, 1, 1);
    Run(0, true, 1, 1, 0);          // wraps at NoBackBuffers + 1
    Run(0, true, 2, 2, 0);
    Run(D3DERR_DEVICELOST, false, 0, 1, 0);
    Run((HRESULT)0x8876086Cl, false, 1, 1, 1);   // other failure: flag untouched
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR PASS\n");
    return 0;
}
