struct CRenderManagerCore
{
    void BeginRender();
};

struct FableDisplayDevice;
typedef long (__stdcall *FableDisplayDeviceCall)(FableDisplayDevice* device);

struct FableDisplayDevice
{
    FableDisplayDeviceCall* vtable;
};

struct CDisplayManagerBeginRenderView
{
    unsigned char unknown000[0x08];
    CRenderManagerCore* renderManagerCore08;
    unsigned char unknown00C[0x4C];
    FableDisplayDevice* displayDevice58;
    unsigned char unknown05C[0x1B8];
    bool renderActive214;

    bool BeginRender();
};

bool CDisplayManagerBeginRenderView::BeginRender()
{
    const long result =
        displayDevice58->vtable[0x29](displayDevice58);
    if (result >= 0)
    {
        renderManagerCore08->BeginRender();
        renderActive214 = true;
        return true;
    }
    return false;
}
