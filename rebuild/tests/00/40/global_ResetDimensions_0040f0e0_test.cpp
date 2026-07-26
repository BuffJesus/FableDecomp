#include <cstdio>
struct Dims { int w; int h; };
struct Device { int w; int h; void GetDims(Dims* out); };
void Device::GetDims(Dims* out) { out->w = this->w; out->h = this->h; }
struct Provider { char pad[0x60]; Device* dev; };
static Device g_devObj = { 640, 480 };
static Provider g_prov;
float g_dimW = 0.0f;
float g_dimH = 0.0f;
Provider* __cdecl GetProvider(void) { g_prov.dev = &g_devObj; return &g_prov; }

void __fastcall NUISystem_CUIScreenUnitConverter_ResetDimensions(void)
{
    Dims d;
    Provider* p = GetProvider();
    p->dev->GetDims(&d);
    g_dimW = (float)d.w;
    g_dimH = (float)d.h;
}

int main(void)
{
    NUISystem_CUIScreenUnitConverter_ResetDimensions();
    if (g_dimW == 640.0f && g_dimH == 480.0f) {
        std::printf("_global_0040f0e0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL w=%f h=%f\n", g_dimW, g_dimH);
    return 1;
}