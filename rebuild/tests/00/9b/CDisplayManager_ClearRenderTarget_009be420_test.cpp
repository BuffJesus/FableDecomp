// Fixture: CDisplayManager::ClearRenderTarget (0x009BE420).
#include <stdio.h>
#include <string.h>
#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_rgb_colour.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

// Table: [0] X8R8G8B8 depth-less; [1] D24S8-like (stencil 8); [2] D24X8 (no stencil); terminator.
extern const CD3DFormatInfo GD3DFormats[] = {
    {(D3DFORMAT)22, PF_TYPE_RGBA, 32, 0, 8, 8, 8, 0, false},
    {(D3DFORMAT)75, PF_TYPE_DEPTH, 32, 0, 0, 0, 0, 8, false},
    {(D3DFORMAT)77, PF_TYPE_DEPTH, 32, 0, 0, 0, 0, 0, false},
    {(D3DFORMAT)102, PF_TYPE_UNKNOWN, -1, 0, 0, 0, 0, 0, false},
};

static int g_findCalls; static D3DFORMAT g_findArg;
long FABLE_FASTCALL CPixelFormat::FindFormatInfoIndexFromD3DFormat(D3DFORMAT f)
{
    ++g_findCalls; g_findArg = f;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i) if (GD3DFormats[i].D3DFormat == f) return i;
    return -1;
}

struct FakeDev { void** vt; };
static void* g_vt[64];
static int g_clearCalls; static void* g_self; static DWORD g_count, g_flags, g_colour, g_stencil; static const void* g_rects; static float g_z;
static HRESULT __stdcall FakeClear(void* self, DWORD count, const void* rects, DWORD flags, DWORD colour, float z, DWORD stencil)
{
    ++g_clearCalls; g_self = self; g_count = count; g_rects = rects; g_flags = flags; g_colour = colour; g_z = z; g_stencil = stencil;
    return 0;
}

static unsigned char g_storage[sizeof(CDisplayManager) + 16];

static void Run(long depthIndex, unsigned long flags, DWORD expectFlags, int expectFind)
{
    CDisplayManager* dm = reinterpret_cast<CDisplayManager*>(g_storage);
    memset(g_storage, 0xCD, sizeof(g_storage));
    static FakeDev dev = { g_vt };
    dm->D3DDevice = reinterpret_cast<IDirect3DDevice9*>(&dev);
    dm->DepthBufferFormatStorage_ = depthIndex;
    g_clearCalls = 0; g_findCalls = 0;
    CRGBColour colour(0x11, 0x22, 0x33, 0x44);
    dm->ClearRenderTarget(colour, flags);
    EXPECT(g_clearCalls == 1);
    EXPECT(g_self == &dev);
    EXPECT(g_count == 0 && g_rects == 0);
    EXPECT(g_flags == expectFlags);
    EXPECT(g_colour == 0x44112233ul);
    EXPECT(g_z == 1.0f && g_stencil == 0);
    EXPECT(g_findCalls == expectFind);
    if (expectFind) EXPECT(g_findArg == GD3DFormats[depthIndex].D3DFormat);
    EXPECT(g_storage[sizeof(CDisplayManager)] == 0xCD);  // untouched neighbour
    EXPECT(dm->DepthBufferFormatStorage_ == depthIndex);
}

int main()
{
    g_vt[43] = (void*)&FakeClear;
    Run(1, 0, 0, 0);
    Run(1, 1, D3DCLEAR_ZBUFFER, 0);
    Run(1, 2, D3DCLEAR_STENCIL, 1);            // stencil format -> stencil flag
    Run(2, 2, 0, 1);                           // no stencil bits -> dropped
    Run(1, 4, D3DCLEAR_TARGET, 0);
    Run(1, 7, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL | D3DCLEAR_TARGET, 1);
    Run(2, 5, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR PASS\n");
    return 0;
}
