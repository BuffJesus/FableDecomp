#include <stdio.h>
#include <string.h>

#include "fable_surface.h"

enum EventKind
{
    EVENT_GET_SOURCE,
    EVENT_GET_DESTINATION,
    EVENT_INITIALISE,
    EVENT_COPY_CONSTRUCT,
    EVENT_UNINITIALISE,
    EVENT_COPY_FROM_SURFACE
};

// Mip surfaces only carry an identity; CSurface's own recorders below do the
// reference counting, so no COM method of these is expected to be called.
struct RawSurface : IDirect3DSurface9
{
    unsigned long id;
    unsigned long addRefCalls;
    unsigned long releaseCalls;

    HRESULT __stdcall QueryInterface(REFIID, void**) { return -1; }
    unsigned long __stdcall AddRef() { return ++addRefCalls; }
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
    HRESULT __stdcall GetDesc(D3DSURFACE_DESC*) { return -1; }
    HRESULT __stdcall LockRect(D3DLOCKED_RECT*, const RECT*, DWORD) { return -1; }
    HRESULT __stdcall UnlockRect() { return -1; }
    HRESULT __stdcall GetDC(void**) { return -1; }
    HRESULT __stdcall ReleaseDC(void*) { return -1; }
};

struct Event
{
    EventKind kind;
    unsigned long surfaceId;
    unsigned long otherSurfaceId;
    unsigned long level;
    unsigned long filter;
    bool destinationBoxWasNull;
    bool sourceBoxWasNull;
};

static Event g_events[256];
static unsigned long g_eventCount;

static void AddEvent(
    EventKind kind,
    unsigned long surfaceId,
    unsigned long otherSurfaceId,
    unsigned long level,
    unsigned long filter,
    bool destinationBoxWasNull,
    bool sourceBoxWasNull)
{
    Event& event = g_events[g_eventCount++];
    event.kind = kind;
    event.surfaceId = surfaceId;
    event.otherSurfaceId = otherSurfaceId;
    event.level = level;
    event.filter = filter;
    event.destinationBoxWasNull = destinationBoxWasNull;
    event.sourceBoxWasNull = sourceBoxWasNull;
}

static RawSurface* AsRaw(IDirect3DSurface9* surface)
{
    return static_cast<RawSurface*>(surface);
}

CSurface::CSurface(const CSurface& source)
    : PD3DSurface(source.PD3DSurface),
      AllocationSource(source.AllocationSource),
      PAllocatedMemory(source.PAllocatedMemory)
{
    RawSurface* raw = AsRaw(PD3DSurface);
    if (raw != 0)
    {
        ++raw->addRefCalls;
    }
    AddEvent(
        EVENT_COPY_CONSTRUCT,
        raw != 0 ? raw->id : 0,
        0,
        0,
        0,
        false,
        false);
}

void CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9* surface)
{
    RawSurface* oldSurface = AsRaw(PD3DSurface);
    if (oldSurface != 0)
    {
        ++oldSurface->releaseCalls;
    }

    PD3DSurface = surface;
    AllocationSource = 3;
    RawSurface* raw = AsRaw(surface);
    AddEvent(
        EVENT_INITIALISE,
        raw != 0 ? raw->id : 0,
        0,
        0,
        0,
        false,
        false);
}

void CSurface::Uninitialise()
{
    RawSurface* raw = AsRaw(PD3DSurface);
    AddEvent(
        EVENT_UNINITIALISE,
        raw != 0 ? raw->id : 0,
        0,
        0,
        0,
        false,
        false);
    if (raw != 0)
    {
        ++raw->releaseCalls;
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}

void CSurface::CopyFromSurface(
    const C2DBoxI* destinationBox,
    const CSurface& source,
    const C2DBoxI* sourceBox,
    unsigned long filter)
{
    RawSurface* destinationRaw = AsRaw(PD3DSurface);
    RawSurface* sourceRaw = AsRaw(source.PD3DSurface);
    AddEvent(
        EVENT_COPY_FROM_SURFACE,
        destinationRaw != 0 ? destinationRaw->id : 0,
        sourceRaw != 0 ? sourceRaw->id : 0,
        0,
        filter,
        destinationBox == 0,
        sourceBox == 0);
}

struct FakeTexture : IDirect3DTexture9
{
    RawSurface surfaces[4];
    unsigned long levelCount;
    bool isSource;
    unsigned long getLevelCalls;

    HRESULT __stdcall QueryInterface(REFIID, void**) { return -1; }
    unsigned long __stdcall AddRef() { return 1; }
    unsigned long __stdcall Release() { return 1; }
    HRESULT __stdcall GetDevice(IDirect3DDevice9**) { return -1; }
    HRESULT __stdcall SetPrivateData(REFGUID, const void*, DWORD, DWORD) { return -1; }
    HRESULT __stdcall GetPrivateData(REFGUID, void*, DWORD*) { return -1; }
    HRESULT __stdcall FreePrivateData(REFGUID) { return -1; }
    DWORD __stdcall SetPriority(DWORD) { return 0; }
    DWORD __stdcall GetPriority() { return 0; }
    void __stdcall PreLoad() {}
    D3DRESOURCETYPE __stdcall GetType() { return D3DRTYPE_TEXTURE; }
    DWORD __stdcall SetLOD(DWORD) { return 0; }
    DWORD __stdcall GetLOD() { return 0; }
    DWORD __stdcall GetLevelCount() { return levelCount; }
    HRESULT __stdcall SetAutoGenFilterType(D3DTEXTUREFILTERTYPE) { return -1; }
    D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType() { return D3DTEXF_NONE; }
    void __stdcall GenerateMipSubLevels() {}
    HRESULT __stdcall GetLevelDesc(UINT, D3DSURFACE_DESC*) { return -1; }
    HRESULT __stdcall GetSurfaceLevel(UINT level, IDirect3DSurface9** surface)
    {
        ++getLevelCalls;
        RawSurface& raw = surfaces[level];
        ++raw.addRefCalls;
        *surface = &raw;
        AddEvent(
            isSource ? EVENT_GET_SOURCE : EVENT_GET_DESTINATION,
            raw.id,
            0,
            level,
            0,
            false,
            false);
        return 0;
    }
    HRESULT __stdcall LockRect(UINT, D3DLOCKED_RECT*, const RECT*, DWORD) { return -1; }
    HRESULT __stdcall UnlockRect(UINT) { return -1; }
    HRESULT __stdcall AddDirtyRect(const RECT*) { return -1; }
};

static void InitialiseFakeTexture(
    FakeTexture& texture,
    bool isSource,
    unsigned long levelCount,
    unsigned long idBase)
{
    texture.getLevelCalls = 0;
    texture.levelCount = levelCount;
    texture.isSource = isSource;
    for (unsigned long level = 0; level != 4; ++level)
    {
        texture.surfaces[level].id = idBase + level;
        texture.surfaces[level].addRefCalls = 0;
        texture.surfaces[level].releaseCalls = 0;
    }
}

static bool CheckMipEvents(
    unsigned long mip,
    unsigned long sourceId,
    unsigned long destinationId,
    unsigned long filter)
{
    const unsigned long base = mip * 11;
    const EventKind expectedKinds[11] = {
        EVENT_GET_SOURCE,
        EVENT_INITIALISE,
        EVENT_COPY_CONSTRUCT,
        EVENT_UNINITIALISE,
        EVENT_GET_DESTINATION,
        EVENT_INITIALISE,
        EVENT_COPY_CONSTRUCT,
        EVENT_UNINITIALISE,
        EVENT_COPY_FROM_SURFACE,
        EVENT_UNINITIALISE,
        EVENT_UNINITIALISE
    };
    const unsigned long expectedIds[11] = {
        sourceId,
        sourceId,
        sourceId,
        sourceId,
        destinationId,
        destinationId,
        destinationId,
        destinationId,
        destinationId,
        destinationId,
        sourceId
    };

    for (unsigned long index = 0; index != 11; ++index)
    {
        const Event& event = g_events[base + index];
        if (
            event.kind != expectedKinds[index] ||
            event.surfaceId != expectedIds[index])
        {
            return false;
        }
    }

    const Event& sourceGet = g_events[base];
    const Event& destinationGet = g_events[base + 4];
    const Event& copy = g_events[base + 8];
    return
        sourceGet.level == mip &&
        destinationGet.level == mip &&
        copy.otherSurfaceId == sourceId &&
        copy.filter == filter &&
        copy.destinationBoxWasNull &&
        copy.sourceBoxWasNull;
}

static int RunMinCase(
    unsigned long sourceLevels,
    unsigned long destinationLevels,
    unsigned long expectedLevels,
    unsigned long filter,
    int failureBase)
{
    FakeTexture source;
    FakeTexture destination;
    InitialiseFakeTexture(source, true, sourceLevels, 100);
    InitialiseFakeTexture(destination, false, destinationLevels, 200);
    g_eventCount = 0;

    CTexture sourceWrapper;
    CTexture destinationWrapper;
    sourceWrapper.PD3DTexture = &source;
    destinationWrapper.PD3DTexture = &destination;
    destinationWrapper.CopyFromTexture(sourceWrapper, filter);

    if (
        source.getLevelCalls != expectedLevels ||
        destination.getLevelCalls != expectedLevels ||
        g_eventCount != expectedLevels * 11)
    {
        return failureBase;
    }

    for (unsigned long level = 0; level != expectedLevels; ++level)
    {
        if (!CheckMipEvents(
                level,
                source.surfaces[level].id,
                destination.surfaces[level].id,
                filter))
        {
            return failureBase + 1;
        }

        if (
            source.surfaces[level].addRefCalls != 2 ||
            source.surfaces[level].releaseCalls != 2 ||
            destination.surfaces[level].addRefCalls != 2 ||
            destination.surfaces[level].releaseCalls != 2)
        {
            return failureBase + 2;
        }
    }

    for (unsigned long level = expectedLevels; level != 4; ++level)
    {
        if (
            source.surfaces[level].addRefCalls != 0 ||
            source.surfaces[level].releaseCalls != 0 ||
            destination.surfaces[level].addRefCalls != 0 ||
            destination.surfaces[level].releaseCalls != 0)
        {
            return failureBase + 3;
        }
    }

    return 0;
}

int main()
{
    int result = RunMinCase(2, 4, 2, 0x12345678, 10);
    if (result != 0)
    {
        printf(
            "FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR FAIL code=%d\n",
            result);
        return result;
    }

    result = RunMinCase(4, 2, 2, 0x87654321, 20);
    if (result != 0)
    {
        printf(
            "FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR FAIL code=%d\n",
            result);
        return result;
    }

    result = RunMinCase(0, 4, 0, 0xFFFFFFFF, 30);
    if (result != 0)
    {
        printf(
            "FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR FAIL code=%d\n",
            result);
        return result;
    }

    printf("FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR PASS\n");
    return 0;
}

// The copy fixture owns mock resources; destruction is outside this probe.
void CTexture::Uninitialise() {}
