#include <math.h>

#include "rebuild_abi.h"

struct C2DExtentsI
{
    fable_i32 width;
    fable_i32 height;
};

struct CDisplayManagerPixelRoundView
{
    C2DExtentsI GetRenderTargetDimensions() const;
};

struct CSystemManagerPixelRoundView
{
    fable_u8 unknown000[0x60];
    CDisplayManagerPixelRoundView* displayManager60;
};

extern double g_FableVirtualHeight_013961F0;

CSystemManagerPixelRoundView* FABLE_FASTCALL GFGetSystemManager();

float FABLE_FASTCALL GFRoundVYToNearestPixel(double y)
{
    CDisplayManagerPixelRoundView* displayManager =
        GFGetSystemManager()->displayManager60;
    const double onePixel =
        g_FableVirtualHeight_013961F0 /
        displayManager->GetRenderTargetDimensions().height;
    const fable_i32 pixel =
        static_cast<fable_i32>(floor(y / onePixel));
    return static_cast<float>(pixel * onePixel);
}
