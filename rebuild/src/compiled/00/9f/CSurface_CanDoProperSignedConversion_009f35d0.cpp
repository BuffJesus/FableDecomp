#include "fable_surface.h"

inline C2DExtentsI CSurface::GetDimensions() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width,desc.Height);
}

inline CPixelFormat CSurface::GetPixelFormat() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormat(desc.Format);
}

bool CSurface::CanDoProperSignedConversion(const C2DBoxI* destinationBox,const C2DBoxI* sourceBox,const CPixelFormat& format)
{
    if(!GetPixelFormat().IsSigned() && !format.IsSigned())return false;
    if(destinationBox && (destinationBox->left!=0 || destinationBox->top!=0 || destinationBox->right!=GetWidth() || destinationBox->bottom!=GetHeight()))return false;
    if(sourceBox && (sourceBox->left!=0 || sourceBox->top!=0 || sourceBox->right!=GetWidth() || sourceBox->bottom!=GetHeight()))return false;
    if(format.IsSigned() || format.GetByteColourDepth()!=4 || !GetPixelFormat().IsSigned() || GetPixelFormat().GetByteColourDepth()!=2)return false;
    return true;
}
