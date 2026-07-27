#include <stdio.h>
#include <string.h>

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

volatile double g_FableVirtualWidth_013961E8 = 1280.0;
static C2DExtentsI g_Dimensions = {640, 480};
static CDisplayManagerCoordinateView g_DisplayManager;
static CSystemManagerCoordinateView g_SystemManager;

C2DExtentsI
CDisplayManagerCoordinateView::GetRenderTargetDimensions() const
{
    return g_Dimensions;
}

CSystemManagerCoordinateView* FABLE_FASTCALL GFGetSystemManager()
{
    return &g_SystemManager;
}

float FABLE_FASTCALL GFAXToVX(float x);

int main()
{
    memset(&g_SystemManager, 0, sizeof(g_SystemManager));
    g_SystemManager.displayManager60 = &g_DisplayManager;

    if (GFAXToVX(10.0f) != 20.0f || GFAXToVX(-3.5f) != -7.0f)
    {
        return 1;
    }

    printf("FABLETLC_GFAX_TO_VX_BEHAVIOR PASS\n");
    return 0;
}
