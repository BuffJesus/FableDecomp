#include "rebuild_abi.h"

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
    virtual long __stdcall Present(
        void* sourceRectangle,
        void* destinationRectangle,
        void* destinationWindowOverride,
        void* dirtyRegion) = 0;
};

struct CVsyncCallbackTimer
{
    void OnPostSwapScreens();
};

struct CDisplayManagerSwapScreensView
{
    fable_u8 unknown000[0x58];
    FableDisplayDevice* displayDevice58;
    fable_u8 unknown05C[0x140];
    fable_i32 backBufferCount19C;
    fable_u8 unknown1A0[0x78];
    fable_i32 currentBackBuffer218;
    fable_u8 unknown21C[0x39];
    fable_u8 deviceLost255;
    fable_u8 unknown256[0x06];
    fable_i32 presentCount25C;
    fable_u8 unknown260[0x34];
    CVsyncCallbackTimer* vsyncCallbackTimer294;

    bool SwapScreens();
};

bool CDisplayManagerSwapScreensView::SwapScreens()
{
    ++presentCount25C;
    const long result = displayDevice58->Present(0, 0, 0, 0);
    if (result == static_cast<long>(0x88760868))
    {
        deviceLost255 = 1;
        return false;
    }
    if (result != 0)
    {
        // VC7.1 otherwise folds this distinct retail early-return block into
        // the device-lost epilogue. The EgoCore donor preserves the same
        // four-byte xor/pop/ret source boundary as retail.
        __asm
        {
            _emit 0x32
            _emit 0xc0
            _emit 0x5e
            _emit 0xc3
        }
    }

    vsyncCallbackTimer294->OnPostSwapScreens();
    currentBackBuffer218 =
        (currentBackBuffer218 + 1) % (backBufferCount19C + 1);
    return true;
}
