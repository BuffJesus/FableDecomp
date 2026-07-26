struct CRenderManagerCore
{
    void EndRender();
};

struct FableDisplayDevice;
typedef long (__stdcall *FableDisplayDeviceCall)(FableDisplayDevice* device);

struct FableDisplayDevice
{
    FableDisplayDeviceCall* vtable;
};

struct CDisplayManagerEndRenderView
{
    unsigned char unknown000[0x08];
    CRenderManagerCore* renderManagerCore08;
    unsigned char unknown00C[0x4C];
    FableDisplayDevice* displayDevice58;
    unsigned char unknown05C[0x1B8];
    bool renderActive214;
    bool renderPending215;

    void EndRender();
};

void CDisplayManagerEndRenderView::EndRender()
{
    renderManagerCore08->EndRender();
    const long result =
        displayDevice58->vtable[0x2A](displayDevice58);
    if (result >= 0)
    {
        renderActive214 = false;
        renderPending215 = false;
    }
}
