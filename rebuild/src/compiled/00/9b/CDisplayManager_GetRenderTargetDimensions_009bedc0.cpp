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

const C2DExtentsI
CDisplayManagerRenderTargetDimensionsView::GetRenderTargetDimensions() const
{
    C2DExtentsI dimensions;
    dimensions.height = renderTargetDimensions194.height;
    dimensions.width = renderTargetDimensions194.width;
    return dimensions;
}
