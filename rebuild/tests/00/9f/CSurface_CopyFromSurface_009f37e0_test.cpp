#include <stdio.h>
#include <string.h>

class C2DBoxI
{
public:
    long left;
    long top;
    long right;
    long bottom;
};

struct CPixelFormat
{
    unsigned long value;
};

class CSurface
{
public:
    void* vtable00;
    void* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    void CopyFromSurface(
        const C2DBoxI* destinationBox,
        const CSurface& source,
        const C2DBoxI* sourceBox,
        unsigned long filter);
};

struct FakeD3DSurface
{
    void** vtable;
    unsigned long format;
    unsigned long width;
    unsigned long height;
    unsigned long getDescCalls;
    unsigned long unlockCalls;
    unsigned long releaseCalls;
};

struct TestState
{
    unsigned long d3dxCalls;
    void* d3dxDestination;
    void* d3dxSource;
    C2DBoxI d3dxDestinationRect;
    C2DBoxI d3dxSourceRect;
    bool d3dxHadDestinationRect;
    bool d3dxHadSourceRect;
    unsigned long d3dxFilter;
    unsigned long canConvertCalls;
    unsigned long initialiseImageCalls;
    unsigned long initialiseWidth;
    unsigned long initialiseHeight;
    unsigned long initialiseFormat;
    unsigned long lockCalls;
    unsigned long conversionCalls;
    const void* conversionData;
    long conversionPitch;
    unsigned long conversionFormat;
};

extern "C" unsigned long g_CSurfaceVTable_0122F84C = 0;
static TestState g_state;
static FakeD3DSurface g_temporarySurface;
static void* g_fakeVTable[15];
static unsigned char g_lockedPixels[64];

static long __stdcall FakeGetDesc(FakeD3DSurface* self, void* output)
{
    unsigned long* description = static_cast<unsigned long*>(output);
    memset(description, 0, 32);
    description[0] = self->format;
    description[6] = self->width;
    description[7] = self->height;
    ++self->getDescCalls;
    return 0;
}

static unsigned long __stdcall FakeRelease(FakeD3DSurface* self)
{
    return ++self->releaseCalls;
}

static long __stdcall FakeUnlock(FakeD3DSurface* self)
{
    ++self->unlockCalls;
    return 0;
}

extern "C" void __fastcall FablePixelFormatInitialise_009E3830(
    CPixelFormat* self,
    void*,
    unsigned long d3dFormat)
{
    self->value = d3dFormat;
}

extern "C" bool __fastcall FablePixelFormatIsSigned_009E3970(
    CPixelFormat* self)
{
    return self->value == 100;
}

extern "C" bool __fastcall FableSurfaceCanDoProperSignedConversion_009F35D0(
    CSurface*,
    void*,
    const C2DBoxI* destinationBox,
    const C2DBoxI* sourceBox,
    const CPixelFormat* sourceFormat)
{
    ++g_state.canConvertCalls;
    return
        destinationBox == 0 &&
        sourceBox == 0 &&
        sourceFormat->value == 0x15;
}

extern "C" bool __fastcall FableSurfaceInitialiseAsImage_009F3230(
    CSurface* self,
    void*,
    const void* dimensions,
    const CPixelFormat* format,
    bool dynamic)
{
    const unsigned long* extents = static_cast<const unsigned long*>(dimensions);
    ++g_state.initialiseImageCalls;
    g_state.initialiseWidth = extents[0];
    g_state.initialiseHeight = extents[1];
    g_state.initialiseFormat = format->value;
    self->surface04 = &g_temporarySurface;
    self->ownership08 = 1;
    return !dynamic;
}

extern "C" void __fastcall FableSurfaceLock_009F33E0(
    CSurface* self,
    void*,
    void* result,
    unsigned long flags)
{
    unsigned long* fields = static_cast<unsigned long*>(result);
    ++g_state.lockCalls;
    fields[2] = 16;
    fields[3] = reinterpret_cast<unsigned long>(g_lockedPixels);
    if (self->surface04 != &g_temporarySurface || flags != 0)
    {
        fields[2] = 0;
        fields[3] = 0;
    }
}

extern "C" void __fastcall
FableSurfaceCopyFromUnsignedDataToSignedSurface_009F36E0(
    CSurface*,
    void*,
    const void* sourceData,
    const CPixelFormat* sourceFormat,
    long sourcePitch)
{
    ++g_state.conversionCalls;
    g_state.conversionData = sourceData;
    g_state.conversionPitch = sourcePitch;
    g_state.conversionFormat = sourceFormat->value;
}

extern "C" long __stdcall FableD3DXLoadSurfaceFromSurface_00BFEFC2(
    void* destinationSurface,
    const void* destinationPalette,
    const void* destinationRect,
    void* sourceSurface,
    const void* sourcePalette,
    const void* sourceRect,
    unsigned long filter,
    unsigned long colourKey)
{
    ++g_state.d3dxCalls;
    g_state.d3dxDestination = destinationSurface;
    g_state.d3dxSource = sourceSurface;
    g_state.d3dxHadDestinationRect = destinationRect != 0;
    g_state.d3dxHadSourceRect = sourceRect != 0;
    if (destinationRect != 0)
    {
        g_state.d3dxDestinationRect =
            *static_cast<const C2DBoxI*>(destinationRect);
    }
    if (sourceRect != 0)
    {
        g_state.d3dxSourceRect = *static_cast<const C2DBoxI*>(sourceRect);
    }
    g_state.d3dxFilter = filter;
    return
        destinationPalette == 0 &&
        sourcePalette == 0 &&
        colourKey == 0 ? 0 : -1;
}

static void InitialiseFakeSurface(
    FakeD3DSurface& surface,
    unsigned long format,
    unsigned long width,
    unsigned long height)
{
    memset(&surface, 0, sizeof(surface));
    surface.vtable = g_fakeVTable;
    surface.format = format;
    surface.width = width;
    surface.height = height;
}

int main()
{
    memset(g_fakeVTable, 0, sizeof(g_fakeVTable));
    g_fakeVTable[2] = reinterpret_cast<void*>(&FakeRelease);
    g_fakeVTable[12] = reinterpret_cast<void*>(&FakeGetDesc);
    g_fakeVTable[14] = reinterpret_cast<void*>(&FakeUnlock);

    FakeD3DSurface destinationResource;
    FakeD3DSurface sourceResource;
    InitialiseFakeSurface(destinationResource, 200, 640, 480);
    InitialiseFakeSurface(sourceResource, 200, 320, 240);

    CSurface destination = {};
    CSurface source = {};
    destination.surface04 = &destinationResource;
    source.surface04 = &sourceResource;

    C2DBoxI destinationBox = {1, 2, 301, 202};
    C2DBoxI sourceBox = {3, 4, 123, 94};
    destination.CopyFromSurface(&destinationBox, source, &sourceBox, 7);
    if (
        g_state.d3dxCalls != 1 ||
        g_state.d3dxDestination != &destinationResource ||
        g_state.d3dxSource != &sourceResource ||
        !g_state.d3dxHadDestinationRect ||
        !g_state.d3dxHadSourceRect ||
        memcmp(
            &g_state.d3dxDestinationRect,
            &destinationBox,
            sizeof(C2DBoxI)) != 0 ||
        memcmp(&g_state.d3dxSourceRect, &sourceBox, sizeof(C2DBoxI)) != 0 ||
        g_state.d3dxFilter != 7 ||
        g_state.conversionCalls != 0)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    memset(&g_state, 0, sizeof(g_state));
    InitialiseFakeSurface(destinationResource, 100, 640, 480);
    InitialiseFakeSurface(sourceResource, 200, 320, 240);
    InitialiseFakeSurface(g_temporarySurface, 200, 640, 480);
    destination.CopyFromSurface(0, source, 0, 5);
    if (
        g_state.canConvertCalls != 1 ||
        g_state.initialiseImageCalls != 1 ||
        g_state.initialiseWidth != 640 ||
        g_state.initialiseHeight != 480 ||
        g_state.initialiseFormat != 0x15 ||
        g_state.d3dxCalls != 1 ||
        g_state.d3dxDestination != &g_temporarySurface ||
        g_state.d3dxSource != &sourceResource ||
        g_state.lockCalls != 1 ||
        g_state.conversionCalls != 1 ||
        g_state.conversionData != g_lockedPixels ||
        g_state.conversionPitch != 16 ||
        g_state.conversionFormat != 0x15 ||
        g_temporarySurface.unlockCalls != 1 ||
        g_temporarySurface.releaseCalls != 1)
    {
        printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR PASS\n");
    return 0;
}
