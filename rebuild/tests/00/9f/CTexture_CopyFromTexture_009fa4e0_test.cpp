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

struct IDirect3DSurface9
{
};

class CSurface
{
public:
    virtual void VTableAnchor();

    IDirect3DSurface9* surface04;
    unsigned long ownership08;
    unsigned long mipLevel0C;

    CSurface(const CSurface& source);
    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);
    void Uninitialise();
    void CopyFromSurface(
        const C2DBoxI* destinationBox,
        const CSurface& source,
        const C2DBoxI* sourceBox,
        unsigned long filter);
};

class CTexture
{
public:
    void* texture00;

    void CopyFromTexture(const CTexture& source, unsigned long filter);
};

extern "C" unsigned long g_CSurfaceVTable_0122F84C = 0;

enum EventKind
{
    EVENT_GET_SOURCE,
    EVENT_GET_DESTINATION,
    EVENT_INITIALISE,
    EVENT_COPY_CONSTRUCT,
    EVENT_UNINITIALISE,
    EVENT_COPY_FROM_SURFACE
};

struct RawSurface : IDirect3DSurface9
{
    unsigned long id;
    unsigned long addRefCalls;
    unsigned long releaseCalls;
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

void CSurface::VTableAnchor()
{
}

CSurface::CSurface(const CSurface& source)
    : surface04(source.surface04),
      ownership08(source.ownership08),
      mipLevel0C(source.mipLevel0C)
{
    RawSurface* raw = AsRaw(surface04);
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
    RawSurface* oldSurface = AsRaw(surface04);
    if (oldSurface != 0)
    {
        ++oldSurface->releaseCalls;
    }

    surface04 = surface;
    ownership08 = 3;
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
    RawSurface* raw = AsRaw(surface04);
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
        surface04 = 0;
        ownership08 = 0;
    }
}

void CSurface::CopyFromSurface(
    const C2DBoxI* destinationBox,
    const CSurface& source,
    const C2DBoxI* sourceBox,
    unsigned long filter)
{
    RawSurface* destinationRaw = AsRaw(surface04);
    RawSurface* sourceRaw = AsRaw(source.surface04);
    AddEvent(
        EVENT_COPY_FROM_SURFACE,
        destinationRaw != 0 ? destinationRaw->id : 0,
        sourceRaw != 0 ? sourceRaw->id : 0,
        0,
        filter,
        destinationBox == 0,
        sourceBox == 0);
}

struct FakeTexture;

typedef unsigned long (__stdcall *GetLevelCountFn)(FakeTexture* self);
typedef long (__stdcall *GetSurfaceLevelFn)(
    FakeTexture* self,
    unsigned long level,
    void** surface);

struct FakeTextureVTable
{
    void* beforeGetLevelCount[13];
    GetLevelCountFn getLevelCount34;
    void* beforeGetSurfaceLevel[4];
    GetSurfaceLevelFn getSurfaceLevel48;
};

struct FakeTexture
{
    FakeTextureVTable* vtable;
    RawSurface surfaces[4];
    unsigned long levelCount;
    bool isSource;
    unsigned long getLevelCalls;
};

static unsigned long __stdcall GetLevelCount(FakeTexture* self)
{
    return self->levelCount;
}

static long __stdcall GetSurfaceLevel(
    FakeTexture* self,
    unsigned long level,
    void** surface)
{
    ++self->getLevelCalls;
    RawSurface& raw = self->surfaces[level];
    ++raw.addRefCalls;
    *surface = &raw;
    AddEvent(
        self->isSource ? EVENT_GET_SOURCE : EVENT_GET_DESTINATION,
        raw.id,
        0,
        level,
        0,
        false,
        false);
    return 0;
}

static FakeTextureVTable g_textureVTable;

static void InitialiseFakeTexture(
    FakeTexture& texture,
    bool isSource,
    unsigned long levelCount,
    unsigned long idBase)
{
    memset(&texture, 0, sizeof(texture));
    texture.vtable = &g_textureVTable;
    texture.levelCount = levelCount;
    texture.isSource = isSource;
    for (unsigned long level = 0; level != 4; ++level)
    {
        texture.surfaces[level].id = idBase + level;
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
    sourceWrapper.texture00 = &source;
    destinationWrapper.texture00 = &destination;
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
    memset(&g_textureVTable, 0, sizeof(g_textureVTable));
    g_textureVTable.getLevelCount34 = GetLevelCount;
    g_textureVTable.getSurfaceLevel48 = GetSurfaceLevel;

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
