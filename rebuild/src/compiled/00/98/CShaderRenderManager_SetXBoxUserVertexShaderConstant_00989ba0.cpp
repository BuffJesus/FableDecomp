// Byte-exact reconstruction of CShaderRenderManager::SetXBoxUserVertexShaderConstant @ 0x00989ba0

struct CRGBFloatColour { float x, y, z, w; };

struct Device;
struct DeviceVtbl {
    char pad[0x178];
    void (__stdcall *SetConstant)(Device* self, long index, const float* data, long count);
};
struct Device {
    DeviceVtbl* vtbl;
};

struct CShaderRenderManager {
    char pad[0x1d4];
    Device* device; // +0x1d4
};

void __fastcall SetXBoxUserVertexShaderConstant(CShaderRenderManager* self, int /*edx*/, long index, CRGBFloatColour* colour)
{
    float buf[4];
    buf[0] = colour->x;
    buf[1] = colour->y;
    buf[2] = colour->z;
    buf[3] = colour->w;
    Device* d = self->device;
    d->vtbl->SetConstant(d, index - 0x60, buf, 1);
}