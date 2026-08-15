#include <cstdio>

struct CRGBFloatColour { float x, y, z, w; };

struct IShaderDevice { void* vtbl; };
struct CShaderRenderManager { char pad[0x1d4]; IShaderDevice* dev; };

typedef void (__stdcall *SetVSConstFn)(IShaderDevice* dev, long reg, float* data, long count);

static long g_reg; static float g_data[4]; static long g_count; static IShaderDevice* g_dev;

void __stdcall capture(IShaderDevice* dev, long reg, float* data, long count) {
    g_dev = dev; g_reg = reg; g_count = count;
    for (int i = 0; i < 4; ++i) g_data[i] = data[i];
}

void __fastcall SetXBoxUserVertexShaderConstant(CShaderRenderManager* self, void* edx, long reg, CRGBFloatColour* colour)
{
    float local[4];
    local[0] = colour->x; local[1] = colour->y; local[2] = colour->z; local[3] = colour->w;
    IShaderDevice* dev = self->dev;
    void** vtbl = *(void***)dev;
    SetVSConstFn fn = (SetVSConstFn)vtbl[0x178/4];
    fn(dev, reg, local, 1);
}

int main() {
    static void* vt[0x60];
    vt[0x178/4] = (void*)&capture;
    IShaderDevice dev; dev.vtbl = vt;
    CShaderRenderManager mgr; mgr.dev = &dev;
    CRGBFloatColour c = { 1.5f, 2.5f, 3.5f, 4.5f };

    SetXBoxUserVertexShaderConstant(&mgr, 0, 7, &c);

    bool ok = (g_dev == &dev) && (g_reg == 7) && (g_count == 1) &&
              g_data[0]==1.5f && g_data[1]==2.5f && g_data[2]==3.5f && g_data[3]==4.5f;
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}