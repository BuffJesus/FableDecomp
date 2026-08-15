struct CRGBFloatColour { float x, y, z, w; };

struct IShaderDevice {
    void* vtbl;
};

struct CShaderRenderManager {
    char pad[0x1d4];
    IShaderDevice* dev;   // +0x1d4
};

typedef void (__stdcall *SetVSConstFn)(IShaderDevice* dev, long reg, float* data, long count);

void __fastcall SetXBoxUserVertexShaderConstant(CShaderRenderManager* self, void* edx, long reg, CRGBFloatColour* colour)
{
    float local[4];
    local[0] = colour->x;
    local[1] = colour->y;
    local[2] = colour->z;
    local[3] = colour->w;
    IShaderDevice* dev = self->dev;
    void** vtbl = *(void***)dev;
    SetVSConstFn fn = (SetVSConstFn)vtbl[0x178/4];
    fn(dev, reg, local, 1);
}