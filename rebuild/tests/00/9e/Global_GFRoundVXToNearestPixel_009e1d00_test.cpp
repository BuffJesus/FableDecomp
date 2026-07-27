#include <math.h>
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

double g_FableVirtualWidth_013961E8 = 1280.0;
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

float FABLE_FASTCALL GFRoundVXToNearestPixel(double x);

int main()
{
    memset(&g_SystemManager, 0, sizeof(g_SystemManager));
    g_SystemManager.displayManager60 = &g_DisplayManager;

    if (
        GFRoundVXToNearestPixel(17.9) != 16.0f ||
        GFRoundVXToNearestPixel(-0.1) != -2.0f ||
        GFRoundVXToNearestPixel(1280.0) != 1280.0f)
    {
        return 1;
    }

    printf("FABLETLC_ROUND_VX_TO_PIXEL_BEHAVIOR PASS\n");
    return 0;
}
