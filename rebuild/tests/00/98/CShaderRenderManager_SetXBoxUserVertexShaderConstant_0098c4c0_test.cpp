#include <cstdio>

struct CRGBFloatColour { float r; float g; float b; };
struct Consts4 { float x; float y; float z; float w; };

struct Captured {
    void* self; long reg; float d0, d1, d2; long d3; long count;
} g_cap;

struct ConstTarget;
typedef void (__stdcall *SetConstFn)(ConstTarget* dev, long reg, void* data, long count);
struct ConstTargetVtbl {
    SetConstFn slots[0x5e];
    SetConstFn SetVertexShaderConstant;
};
struct ConstTarget { ConstTargetVtbl* vtbl; };

struct CShaderRenderManager {
    char pad[0x1d4];
    ConstTarget* device;
};

void __fastcall SetXBoxUserVertexShaderConstant(CShaderRenderManager* self, void* edx, long reg, CRGBFloatColour* col, long w)
{
    Consts4 c;
    c.x = col->r;
    c.y = col->g;
    c.z = col->b;
    *(long*)&c.w = w;
    ConstTarget* dev = self->device;
    dev->vtbl->SetVertexShaderConstant(dev, reg, &c, 1);
}

void __stdcall MySet(ConstTarget* self, long reg, void* data, long count)
{
    Consts4* c = (Consts4*)data;
    g_cap.self = self;
    g_cap.reg = reg;
    g_cap.d0 = c->x; g_cap.d1 = c->y; g_cap.d2 = c->z;
    g_cap.d3 = *(long*)&c->w;
    g_cap.count = count;
}

int main()
{
    ConstTargetVtbl vt;
    vt.SetVertexShaderConstant = MySet;
    ConstTarget dev; dev.vtbl = &vt;
    CShaderRenderManager mgr;
    mgr.device = &dev;

    CRGBFloatColour col; col.r = 1.5f; col.g = 2.5f; col.b = 3.5f;
    long reg = 42;
    long w = 99;

    SetXBoxUserVertexShaderConstant(&mgr, 0, reg, &col, w);

    bool ok = (g_cap.self == &dev) && (g_cap.reg == 42) &&
              (g_cap.d0 == 1.5f) && (g_cap.d1 == 2.5f) && (g_cap.d2 == 3.5f) &&
              (g_cap.d3 == 99) && (g_cap.count == 1);
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL self=%p reg=%ld d=%f,%f,%f w=%ld cnt=%ld\n",
        g_cap.self, g_cap.reg, g_cap.d0, g_cap.d1, g_cap.d2, g_cap.d3, g_cap.count);
    return 0;
}