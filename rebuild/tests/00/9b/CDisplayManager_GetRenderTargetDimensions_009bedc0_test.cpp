#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct C2DExtentsI
{
    fable_i32 width;
    fable_i32 height;
};

struct CDisplayManagerRenderTargetDimensionsView
{
    fable_u8 unknown000[0x194];
    C2DExtentsI renderTargetDimensions194;

    const C2DExtentsI GetRenderTargetDimensions() const;
};

int main()
{
    CDisplayManagerRenderTargetDimensionsView displayManager;
    memset(&displayManager, 0, sizeof(displayManager));
    displayManager.renderTargetDimensions194.width = 1280;
    displayManager.renderTargetDimensions194.height = 720;

    const C2DExtentsI dimensions =
        displayManager.GetRenderTargetDimensions();
    if (dimensions.width != 1280 || dimensions.height != 720)
    {
        return 1;
    }

    printf("FABLETLC_RENDER_TARGET_DIMENSIONS_BEHAVIOR PASS\n");
    return 0;
}
