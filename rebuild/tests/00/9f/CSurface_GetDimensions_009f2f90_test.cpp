// Fixture: CSurface::GetDimensions (0x009F2F90).
#include <stdio.h>
#include <string.h>
#include "fable_surface.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

struct FakeSurf { void** vt; };
static void* g_vt[32];
static FakeSurf g_surf = { g_vt };
static int g_descCalls; static void* g_descSelf;
static UINT g_w = 640, g_h = 480; static D3DFORMAT g_fmt = (D3DFORMAT)22;
static HRESULT __stdcall FakeGetDesc(void* self, D3DSURFACE_DESC* d)
{
    ++g_descCalls; g_descSelf = self;
    memset(d, 0x5A, sizeof(*d));
    d->Width = g_w; d->Height = g_h; d->Format = g_fmt;
    return 0;
}
static unsigned char g_obj[sizeof(CSurface) + 8];
static CSurface* MakeSurface()
{
    memset(g_obj, 0xCD, sizeof(g_obj));
    CSurface* s = reinterpret_cast<CSurface*>(g_obj);
    s->PD3DSurface = reinterpret_cast<IDirect3DSurface9*>(&g_surf);
    g_vt[12] = (void*)&FakeGetDesc;
    g_descCalls = 0;
    return s;
}
int main()
{
    CSurface* s = MakeSurface();
    C2DExtentsI e = s->GetDimensions();
    EXPECT(g_descCalls == 1 && g_descSelf == &g_surf);
    EXPECT(e.X == 640 && e.Y == 480);
    g_w = 17; g_h = 9;
    e = s->GetDimensions();
    EXPECT(g_descCalls == 2 && e.X == 17 && e.Y == 9);
    EXPECT(g_obj[sizeof(CSurface)] == 0xCD && s->PD3DSurface == (void*)&g_surf);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("SURFACE_DIMENSIONS PASS\n");
    return 0;
}
