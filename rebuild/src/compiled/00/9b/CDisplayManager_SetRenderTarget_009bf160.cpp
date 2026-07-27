#include "rebuild_abi.h"

struct C2DExtentsI
{
    fable_i32 width;
    fable_i32 height;
};

struct C2DBoxI
{
    fable_i32 left;
    fable_i32 top;
    fable_i32 right;
    fable_i32 bottom;
};

struct CSurface
{
    void* vtable00;
    void* resource04;
    fable_u32 resourceState08;
    fable_u32 unknown0C;

    CSurface& operator=(const CSurface& source);
    C2DExtentsI GetDimensions() const;

    __forceinline void* GetSurface() const
    {
        return resource04;
    }
};

struct CRenderTarget
{
    void* vtable00;
    CSurface renderColour04;
    CSurface renderDepth14;
};

struct FableDisplayDevice
{
    virtual void __stdcall Slot00() = 0;
    virtual void __stdcall Slot01() = 0;
    virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0;
    virtual void __stdcall Slot04() = 0;
    virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0;
    virtual void __stdcall Slot07() = 0;
    virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0;
    virtual void __stdcall Slot0A() = 0;
    virtual void __stdcall Slot0B() = 0;
    virtual void __stdcall Slot0C() = 0;
    virtual void __stdcall Slot0D() = 0;
    virtual void __stdcall Slot0E() = 0;
    virtual void __stdcall Slot0F() = 0;
    virtual void __stdcall Slot10() = 0;
    virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual void __stdcall Slot13() = 0;
    virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0;
    virtual void __stdcall Slot16() = 0;
    virtual void __stdcall Slot17() = 0;
    virtual void __stdcall Slot18() = 0;
    virtual void __stdcall Slot19() = 0;
    virtual void __stdcall Slot1A() = 0;
    virtual void __stdcall Slot1B() = 0;
    virtual void __stdcall Slot1C() = 0;
    virtual void __stdcall Slot1D() = 0;
    virtual void __stdcall Slot1E() = 0;
    virtual void __stdcall Slot1F() = 0;
    virtual void __stdcall Slot20() = 0;
    virtual void __stdcall Slot21() = 0;
    virtual void __stdcall Slot22() = 0;
    virtual void __stdcall Slot23() = 0;
    virtual void __stdcall Slot24() = 0;
    virtual long __stdcall SetRenderTarget(
        fable_u32 index,
        void* surface) = 0;
    virtual void __stdcall Slot26() = 0;
    virtual long __stdcall SetDepthStencilSurface(
        void* surface) = 0;
};

struct CDisplayManagerSetRenderTargetView
{
    fable_u8 unknown000[0x0C];
    CSurface renderColour0C;
    CSurface renderDepth1C;
    fable_u8 unknown02C[0x2C];
    FableDisplayDevice* displayDevice58;
    fable_u8 unknown05C[0x138];
    C2DExtentsI renderTargetDimensions194;

    void InitVirtualPixels();
    void SetViewport(const C2DBoxI& viewport);
    bool SetRenderTarget(CRenderTarget& target);
};

bool CDisplayManagerSetRenderTargetView::SetRenderTarget(
    CRenderTarget& target)
{
    CSurface* renderColour = &renderColour0C;
    *renderColour = target.renderColour04;
    CSurface* targetDepth = &target.renderDepth14;
    renderDepth1C = *targetDepth;
    renderTargetDimensions194 = renderColour->GetDimensions();
    InitVirtualPixels();

    void* depthSurface = 0;
    if (targetDepth != 0)
        depthSurface = target.renderDepth14.GetSurface();

    if (displayDevice58->SetRenderTarget(
            0,
            renderColour0C.GetSurface()) >= 0)
    {
        if (displayDevice58->SetDepthStencilSurface(
                depthSurface) >= 0)
        {
            C2DBoxI viewport = {
                0,
                0,
                renderTargetDimensions194.width,
                renderTargetDimensions194.height
            };
            SetViewport(viewport);
            return true;
        }
    }
    return false;
}
