struct Dims { int w; int h; };
struct Device { void GetDims(Dims* out); };
struct Provider { char pad[0x60]; Device* dev; };
extern Provider* __cdecl GetProvider(void);
extern float g_dimW;
extern float g_dimH;

void __fastcall NUISystem_CUIScreenUnitConverter_ResetDimensions(void)
{
    Dims d;
    Provider* p = GetProvider();
    p->dev->GetDims(&d);
    g_dimW = (float)d.w;
    g_dimH = (float)d.h;
}