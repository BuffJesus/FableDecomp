#include <stdio.h>
#include <string.h>

#include "fable_surface.h"

// Formats are modelled by value: 100 is the one "signed" format.
static const long kSignedFormat = 100;

struct FakeD3DSurface : IDirect3DSurface9
{
    D3DFORMAT format;
    UINT width;
    UINT height;
    unsigned long getDescCalls;
    unsigned long unlockCalls;
    unsigned long releaseCalls;

    HRESULT __stdcall QueryInterface(REFIID, void**) { return -1; }
    unsigned long __stdcall AddRef() { return 1; }
    unsigned long __stdcall Release() { return ++releaseCalls; }
    HRESULT __stdcall GetDevice(IDirect3DDevice9**) { return -1; }
    HRESULT __stdcall SetPrivateData(REFGUID, const void*, DWORD, DWORD) { return -1; }
    HRESULT __stdcall GetPrivateData(REFGUID, void*, DWORD*) { return -1; }
    HRESULT __stdcall FreePrivateData(REFGUID) { return -1; }
    DWORD __stdcall SetPriority(DWORD) { return 0; }
    DWORD __stdcall GetPriority() { return 0; }
    void __stdcall PreLoad() {}
    D3DRESOURCETYPE __stdcall GetType() { return D3DRTYPE_SURFACE; }
    HRESULT __stdcall GetContainer(REFIID, void**) { return -1; }
    HRESULT __stdcall GetDesc(D3DSURFACE_DESC* desc)
    {
        memset(desc, 0, sizeof(*desc));
        desc->Format = format;
        desc->Width = width;
        desc->Height = height;
        ++getDescCalls;
        return 0;
    }
    HRESULT __stdcall LockRect(D3DLOCKED_RECT*, const RECT*, DWORD) { return -1; }
    HRESULT __stdcall UnlockRect()
    {
        ++unlockCalls;
        return 0;
    }
    HRESULT __stdcall GetDC(void**) { return -1; }
    HRESULT __stdcall ReleaseDC(void*) { return -1; }
};

struct TestState
{
    unsigned long d3dxCalls;
    IDirect3DSurface9* d3dxDestination;
    IDirect3DSurface9* d3dxSource;
    RECT d3dxDestinationRect;
    RECT d3dxSourceRect;
    bool d3dxHadDestinationRect;
    bool d3dxHadSourceRect;
    unsigned long d3dxFilter;
    unsigned long canConvertCalls;
    unsigned long initialiseImageCalls;
    long initialiseWidth;
    long initialiseHeight;
    long initialiseFormat;
    unsigned long lockCalls;
    unsigned long conversionCalls;
    const void* conversionData;
    long conversionPitch;
    long conversionFormat;
};

static TestState g_state;
static FakeD3DSurface g_temporarySurface;
static unsigned char g_lockedPixels[64];

void CPixelFormat::Initialise(D3DFORMAT format)
{
    FormatIndex = format;
}

bool CPixelFormat::IsSigned() const
{
    return FormatIndex == kSignedFormat;
}

bool CSurface::CanDoProperSignedConversion(
    const C2DBoxI* destinationBox,
    const C2DBoxI* sourceBox,
    const CPixelFormat& format)
{
    ++g_state.canConvertCalls;
    return
        destinationBox == 0 &&
        sourceBox == 0 &&
        format.FormatIndex == D3DFMT_A8R8G8B8;
}

bool CSurface::InitialiseAsImage(
    const C2DExtentsI& dimensions,
    const CPixelFormat& format,
    bool dynamic)
{
    ++g_state.initialiseImageCalls;
    g_state.initialiseWidth = dimensions.X;
    g_state.initialiseHeight = dimensions.Y;
    g_state.initialiseFormat = format.FormatIndex;
    PD3DSurface = &g_temporarySurface;
    AllocationSource = 1;
    return !dynamic;
}

CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    CSurfaceLockedRect result;
    ++g_state.lockCalls;
    result.Dimensions = C2DExtentsI(0, 0);
    result.Pitch = 16;
    result.PMem = g_lockedPixels;
    if (PD3DSurface != &g_temporarySurface || flags != 0)
    {
        result.Pitch = 0;
        result.PMem = 0;
    }
    return result;
}

void CSurface::CopyFromUnsignedDataToSignedSurface(
    const void* data,
    const CPixelFormat& format,
    long pitch)
{
    ++g_state.conversionCalls;
    g_state.conversionData = data;
    g_state.conversionPitch = pitch;
    g_state.conversionFormat = format.FormatIndex;
}

extern "C" HRESULT __stdcall D3DXLoadSurfaceFromSurface(
    IDirect3DSurface9* destinationSurface,
    const PALETTEENTRY* destinationPalette,
    const RECT* destinationRect,
    IDirect3DSurface9* sourceSurface,
    const PALETTEENTRY* sourcePalette,
    const RECT* sourceRect,
    DWORD filter,
    D3DCOLOR colourKey)
{
    ++g_state.d3dxCalls;
    g_state.d3dxDestination = destinationSurface;
    g_state.d3dxSource = sourceSurface;
    g_state.d3dxHadDestinationRect = destinationRect != 0;
    g_state.d3dxHadSourceRect = sourceRect != 0;
    if (destinationRect != 0)
    {
        g_state.d3dxDestinationRect = *destinationRect;
    }
    if (sourceRect != 0)
    {
        g_state.d3dxSourceRect = *sourceRect;
    }
    g_state.d3dxFilter = filter;
    return
        destinationPalette == 0 &&
        sourcePalette == 0 &&
        colourKey == 0 ? 0 : -1;
}

static void InitialiseFakeSurface(
    FakeD3DSurface& surface,
    long format,
    UINT width,
    UINT height)
{
    surface.format = static_cast<D3DFORMAT>(format);
    surface.width = width;
    surface.height = height;
    surface.getDescCalls = 0;
    surface.unlockCalls = 0;
    surface.releaseCalls = 0;
}

static bool SameRect(const RECT& rect, const C2DBoxI& box)
{
    return
        rect.left == box.left &&
        rect.top == box.top &&
        rect.right == box.right &&
        rect.bottom == box.bottom;
}

int main()
{
    FakeD3DSurface destinationResource;
    FakeD3DSurface sourceResource;
    InitialiseFakeSurface(destinationResource, 200, 640, 480);
    InitialiseFakeSurface(sourceResource, 200, 320, 240);

    CSurface destination;
    CSurface source;
    destination.PD3DSurface = &destinationResource;
    source.PD3DSurface = &sourceResource;

    // Unsigned destination: one D3DX copy with both boxes as RECTs.
    C2DBoxI destinationBox = {1, 2, 301, 202};
    C2DBoxI sourceBox = {3, 4, 123, 94};
    destination.CopyFromSurface(&destinationBox, source, &sourceBox, 7);
    if (
        g_state.d3dxCalls != 1 ||
        g_state.d3dxDestination != &destinationResource ||
        g_state.d3dxSource != &sourceResource ||
        !g_state.d3dxHadDestinationRect ||
        !g_state.d3dxHadSourceRect ||
        !SameRect(g_state.d3dxDestinationRect, destinationBox) ||
        !SameRect(g_state.d3dxSourceRect, sourceBox) ||
        g_state.d3dxFilter != 7 ||
        g_state.canConvertCalls != 0 ||
        g_state.conversionCalls != 0)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    // Null boxes stay null.
    memset(&g_state, 0, sizeof(g_state));
    destination.CopyFromSurface(0, source, 0, 3);
    if (
        g_state.d3dxCalls != 1 ||
        g_state.d3dxHadDestinationRect ||
        g_state.d3dxHadSourceRect ||
        g_state.d3dxFilter != 3)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    // Signed destination, unsigned source: copy through an A8R8G8B8 image
    // of the destination's size (default filter), then convert its bits.
    memset(&g_state, 0, sizeof(g_state));
    InitialiseFakeSurface(destinationResource, kSignedFormat, 640, 480);
    InitialiseFakeSurface(sourceResource, 200, 320, 240);
    InitialiseFakeSurface(g_temporarySurface, 200, 640, 480);
    destination.CopyFromSurface(&destinationBox, source, &sourceBox, 5);
    if (
        g_state.canConvertCalls != 1 ||
        g_state.initialiseImageCalls != 1 ||
        g_state.initialiseWidth != 640 ||
        g_state.initialiseHeight != 480 ||
        g_state.initialiseFormat != D3DFMT_A8R8G8B8 ||
        g_state.d3dxCalls != 1 ||
        g_state.d3dxDestination != &g_temporarySurface ||
        g_state.d3dxSource != &sourceResource ||
        !SameRect(g_state.d3dxDestinationRect, destinationBox) ||
        !SameRect(g_state.d3dxSourceRect, sourceBox) ||
        g_state.d3dxFilter != 0xffffffff ||
        g_state.lockCalls != 1 ||
        g_state.conversionCalls != 1 ||
        g_state.conversionData != g_lockedPixels ||
        g_state.conversionPitch != 16 ||
        g_state.conversionFormat != D3DFMT_A8R8G8B8 ||
        g_temporarySurface.unlockCalls != 1 ||
        g_temporarySurface.releaseCalls != 1 ||
        destinationResource.releaseCalls != 0)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    // Signed on both sides: straight D3DX copy.
    memset(&g_state, 0, sizeof(g_state));
    InitialiseFakeSurface(sourceResource, kSignedFormat, 320, 240);
    destination.CopyFromSurface(0, source, 0, 9);
    if (
        g_state.canConvertCalls != 0 ||
        g_state.d3dxCalls != 1 ||
        g_state.d3dxDestination != &destinationResource ||
        g_state.conversionCalls != 0)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR PASS\n");
    return 0;
}
