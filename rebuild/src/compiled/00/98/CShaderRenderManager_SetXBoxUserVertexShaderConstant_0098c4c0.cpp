struct CRGBFloatColour { float r; float g; float b; };
struct Consts4 { float x; float y; float z; float w; };

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