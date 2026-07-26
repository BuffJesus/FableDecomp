#include "rebuild_abi.h"

struct C2DExtentsI
{
    fable_i32 width;
    fable_i32 height;
};

struct CDisplayManagerCoordinateView
{
    C2DExtentsI GetRenderTargetDimensions() const;
};

struct CSystemManagerCoordinateView
{
    fable_u8 unknown000[0x60];
    CDisplayManagerCoordinateView* displayManager60;
};

extern volatile double g_FableVirtualWidth_013961E8;

CSystemManagerCoordinateView* FABLE_FASTCALL GFGetSystemManager();

float FABLE_FASTCALL GFAXToVX(float x)
{
    CDisplayManagerCoordinateView* displayManager =
        GFGetSystemManager()->displayManager60;
    return static_cast<float>(
        (g_FableVirtualWidth_013961E8 * x) /
        displayManager->GetRenderTargetDimensions().width);
}
