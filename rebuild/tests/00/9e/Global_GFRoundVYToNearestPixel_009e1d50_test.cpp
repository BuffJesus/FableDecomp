#include <stdio.h>
#include <string.h>

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

double g_FableVirtualHeight_013961F0 = 960.0;
static C2DExtentsI g_Dimensions = {640, 480};
static CDisplayManagerPixelRoundView g_DisplayManager;
static CSystemManagerPixelRoundView g_SystemManager;

C2DExtentsI
CDisplayManagerPixelRoundView::GetRenderTargetDimensions() const
{
    return g_Dimensions;
}

CSystemManagerPixelRoundView* FABLE_FASTCALL GFGetSystemManager()
{
    return &g_SystemManager;
}

float FABLE_FASTCALL GFRoundVYToNearestPixel(double y);

int main()
{
    memset(&g_SystemManager, 0, sizeof(g_SystemManager));
    g_SystemManager.displayManager60 = &g_DisplayManager;

    if (
        GFRoundVYToNearestPixel(9.9) != 8.0f ||
        GFRoundVYToNearestPixel(-0.1) != -2.0f ||
        GFRoundVYToNearestPixel(960.0) != 960.0f)
    {
        return 1;
    }

    printf("FABLETLC_ROUND_VY_TO_PIXEL_BEHAVIOR PASS\n");
    return 0;
}
