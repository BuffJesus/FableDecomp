#include "fable_surface.h"

// CSurface.cpp: the members CopyFromSurface inlines are defined above it, as
// in the retail object (0x9F2E20 Uninitialise, 0x9F2F90 GetDimensions).

void CSurface::Uninitialise()
{
    if (PD3DSurface != 0)
    {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}

inline C2DExtentsI CSurface::GetDimensions() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width, desc.Height);
}

inline CPixelFormat CSurface::GetPixelFormat() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormat(desc.Format);
}

void CSurface::CopyFromSurface(
    const C2DBoxI* destinationBox,
    const CSurface& source,
    const C2DBoxI* sourceBox,
    unsigned long filter)
{
    // D3DX cannot fill a signed surface from unsigned data: copy into an
    // unsigned A8R8G8B8 image first and convert that by hand.
    if (GetPixelFormat().IsSigned() &&
        !source.GetPixelFormat().IsSigned() &&
        CanDoProperSignedConversion(0, 0, CPixelFormat(D3DFMT_A8R8G8B8)))
    {
        CPixelFormat unsignedFormat;
        unsignedFormat.Initialise(D3DFMT_A8R8G8B8);

        CSurface unsignedSurface;
        unsignedSurface.InitialiseAsImage(GetDimensions(), unsignedFormat, false);
        unsignedSurface.CopyFromSurface(destinationBox, source, sourceBox, 0xffffffff);

        {
            CSurfaceLockedRect lockedRect = unsignedSurface.Lock(0);
            CopyFromUnsignedDataToSignedSurface(lockedRect.PMem, unsignedFormat, lockedRect.Pitch);
        }
        unsignedSurface.Unlock();
    }
    else
    {
        RECT destinationRect;
        RECT sourceRect;
        const RECT* destinationRectPtr = 0;
        const RECT* sourceRectPtr = 0;
        if (destinationBox != 0)
        {
            destinationRect.top = destinationBox->top;
            destinationRect.bottom = destinationBox->bottom;
            destinationRect.left = destinationBox->left;
            destinationRect.right = destinationBox->right;
            destinationRectPtr = &destinationRect;
        }
        if (sourceBox != 0)
        {
            sourceRect.top = sourceBox->top;
            sourceRect.bottom = sourceBox->bottom;
            sourceRect.left = sourceBox->left;
            sourceRect.right = sourceBox->right;
            sourceRectPtr = &sourceRect;
        }

        D3DXLoadSurfaceFromSurface(
            PD3DSurface, 0, destinationRectPtr,
            source.PD3DSurface, 0, sourceRectPtr,
            filter, 0);
    }
}
