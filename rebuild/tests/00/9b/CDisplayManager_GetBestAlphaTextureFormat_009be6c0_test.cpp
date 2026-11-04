// Fixture: CDisplayManager::GetBestAlphaTextureFormat (0x009BE6C0)
#include <stdio.h>
#include <string.h>
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

extern const CD3DFormatInfo GD3DFormats[] = {
    {(D3DFORMAT)20, PF_TYPE_RGBA, 24, 0, 8, 8, 8, 0, false},
    {(D3DFORMAT)21, PF_TYPE_RGBA, 32, 8, 8, 8, 8, 0, false},
    {(D3DFORMAT)22, PF_TYPE_RGBA, 32, 0, 8, 8, 8, 0, false},
    {(D3DFORMAT)23, PF_TYPE_RGBA, 16, 0, 5, 6, 5, 0, false},
    {(D3DFORMAT)25, PF_TYPE_RGBA, 16, 1, 5, 5, 5, 0, false},
    {(D3DFORMAT)26, PF_TYPE_RGBA, 16, 4, 4, 4, 4, 0, false},
    {(D3DFORMAT)60, PF_TYPE_BUMP, 16, 0, 8, 8, 0, 0, false},
    {(D3DFORMAT)61, PF_TYPE_BUMP, 16, 0, 5, 5, 6, 0, false},
    {(D3DFORMAT)64, PF_TYPE_BUMP, 32, 0, 16, 16, 0, 0, false},
    {(D3DFORMAT)75, PF_TYPE_DEPTH, 16, 0, 0, 0, 0, 8, false},
    {(D3DFORMAT)28, PF_TYPE_RGBA, 16, 8, 0, 0, 0, 0, false},
    {(D3DFORMAT)102, PF_TYPE_UNKNOWN, -1, 0, 0, 0, 0, 0, false},
};

// Fake IDirect3D9: slot 10 = CheckDeviceFormat.
struct FakeD3D { void** vt; };
static void* g_vt[32];
static FakeD3D g_d3d = { g_vt };
static int g_checks; static long g_supported[8]; static int g_nSupported;
static D3DFORMAT g_checked[32]; static DWORD g_usage[32];
static bool g_argsOk = true;
static HRESULT __stdcall FakeCheck(void* self, UINT adapter, D3DDEVTYPE type, D3DFORMAT adapterFormat,
                                   DWORD usage, D3DRESOURCETYPE rtype, D3DFORMAT fmt)
{
    if (self != &g_d3d || adapter != 7 || type != D3DDEVTYPE_REF || adapterFormat != (D3DFORMAT)22 || rtype != D3DRTYPE_TEXTURE)
        g_argsOk = false;
    g_checked[g_checks] = fmt; g_usage[g_checks] = usage; ++g_checks;
    for (int i = 0; i < g_nSupported; ++i) if (g_supported[i] == (long)fmt) return 0;
    return (HRESULT)0x8876086Al;
}

static int g_inits; static CPixelFormat* g_initThis; static D3DFORMAT g_initFmt;
void CPixelFormat::Initialise(D3DFORMAT f) { ++g_inits; g_initThis = this; g_initFmt = f; FormatIndex = 1000 + (long)f; }

static unsigned char g_storage[sizeof(CDisplayManager) + 16];
static CDisplayManager* Setup(int n, const long* supported)
{
    memset(g_storage, 0xCD, sizeof(g_storage));
    CDisplayManager* dm = reinterpret_cast<CDisplayManager*>(g_storage);
    g_vt[10] = (void*)&FakeCheck;
    dm->D3D = reinterpret_cast<IDirect3D9*>(&g_d3d);
    dm->D3DDeviceCaps.AdapterOrdinal = 7;
    dm->D3DDeviceCaps.DeviceType = D3DDEVTYPE_REF;
    dm->CurrentMode.Format = (D3DFORMAT)22;
    g_nSupported = n; for (int i = 0; i < n; ++i) g_supported[i] = supported[i];
    g_checks = 0; g_inits = 0; g_initThis = 0; g_initFmt = (D3DFORMAT)0; g_argsOk = true;
    return dm;
}
static void Neighbour() { EXPECT(g_storage[sizeof(CDisplayManager)] == 0xCD); EXPECT(g_argsOk); }

static void Body()
{
    CPixelFormat format; format.FormatIndex = -5;
    long all[] = { 23, 25, 26, 21 };
    CDisplayManager* dm = Setup(4, all);
    EXPECT(dm->GetBestAlphaTextureFormat(16, format) == true);
    // Needs >1 bit in every channel: only 26 qualifies at 16 bits.
    EXPECT(g_checks == 1 && g_checked[0] == 26 && g_usage[0] == 0);
    EXPECT(g_inits == 1 && g_initThis == &format && g_initFmt == 26);
    Neighbour();
    dm = Setup(4, all);
    EXPECT(dm->GetBestAlphaTextureFormat(32, format) == true && g_checks == 1 && g_initFmt == 21);
    dm = Setup(0, all);
    EXPECT(dm->GetBestAlphaTextureFormat(16, format) == false && g_checks == 1 && g_inits == 0);
}

int main()
{
    Body();
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_DISPLAY_BEST_ALPHA_FORMAT PASS\n");
    return 0;
}