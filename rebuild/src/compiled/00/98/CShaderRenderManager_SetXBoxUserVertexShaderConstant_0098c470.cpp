struct CRGBFloatColour { float x, y, z; };

struct IDevice;
typedef void (__stdcall *SetConstFn)(IDevice* self, int reg, void* pData, int count);
struct IDeviceVtbl { void* slots[0x5f]; };
struct IDevice { IDeviceVtbl* vtbl; };

struct CShaderRenderManager {
    char pad[0x1d4];
    IDevice* device; // +0x1d4
    void SetXBoxUserVertexShaderConstant(int reg, CRGBFloatColour* colour, int packedIdx);
};

void CShaderRenderManager::SetXBoxUserVertexShaderConstant(
    int reg, CRGBFloatColour* colour, int packedIdx)
{
    float local[4];
    void* pdup = &packedIdx;
    local[0] = colour->x;
    local[1] = colour->y;
    local[2] = colour->z;
    local[3] = *(float*)pdup;
    IDevice* dev = this->device;
    SetConstFn fn = (SetConstFn)dev->vtbl->slots[0x178/4];
    fn(dev, reg, &local[0], 1);
}