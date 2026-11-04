#include "fable_surface.h"
#include "fable_d3d_format_info.h"
extern "C" HRESULT __stdcall D3DXLoadSurfaceFromMemory(IDirect3DSurface9*,const PALETTEENTRY*,const RECT*,const void*,D3DFORMAT,UINT,const PALETTEENTRY*,const RECT*,DWORD,D3DCOLOR);
inline C2DExtentsI CSurface::GetDimensions() const { D3DSURFACE_DESC desc; PD3DSurface->GetDesc(&desc); return C2DExtentsI(desc.Width,desc.Height); }
inline unsigned long CSurface::GetHeight() const { return GetDimensions().Y; }
inline CPixelFormat CSurface::GetPixelFormat() const { D3DSURFACE_DESC desc; PD3DSurface->GetDesc(&desc); return CPixelFormat(desc.Format); }
void CSurface::CopyFromMemory(const C2DBoxI* destinationBox,const void* data,const C2DBoxI* sourceBox,const CPixelFormat& format,long pitch,unsigned long filter)
{
    long destinationWidth=destinationBox ? destinationBox->right-destinationBox->left : GetWidth();
    long destinationHeight=destinationBox ? destinationBox->bottom-destinationBox->top : GetHeight();
    long sourceWidth=sourceBox ? sourceBox->right-sourceBox->left : destinationWidth;
    long sourceHeight=sourceBox ? sourceBox->bottom-sourceBox->top : destinationHeight;
    if((GetPixelFormat().IsSigned() || format.IsSigned()) &&
       !(GetPixelFormat().IsSigned() && format.IsSigned()) &&
       CanDoProperSignedConversion(destinationBox,sourceBox,format)) {
        CopyFromUnsignedDataToSignedSurface(data,format,pitch);
        return;
    }
    if(format.IsEquivalent(GetPixelFormat()) && sourceWidth==destinationWidth && sourceHeight==destinationHeight) {
        CopyFromMemory(destinationBox,data,sourceBox,pitch);
        return;
    }
    RECT destination,source;const RECT* dp=0;const RECT* sp=0;
    if(destinationBox) { destination.left=destinationBox->left;destination.top=destinationBox->top;destination.right=destinationBox->right;destination.bottom=destinationBox->bottom;dp=&destination; }
    if(sourceBox) { source.left=sourceBox->left;source.top=sourceBox->top;source.right=sourceBox->right;source.bottom=sourceBox->bottom;sp=&source; }
    D3DXLoadSurfaceFromMemory(PD3DSurface,0,dp,data,GD3DFormats[format.FormatIndex].D3DFormat,pitch,0,sp,filter,0);
}
