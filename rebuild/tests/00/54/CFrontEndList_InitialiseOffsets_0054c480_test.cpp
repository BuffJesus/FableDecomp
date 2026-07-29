// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndList::InitialiseOffsets @ 0x0054C480.

#include <cstdio>
#include <cstring>

struct Vector2
{
    float x;
    float y;
};

struct FrontEndListProbe
{
    void** vtable;
    unsigned char untouched[0x200];
};

extern "C" void __fastcall
CFrontEndList_InitialiseOffsets_0054c480(
    void* self,
    void*,
    unsigned long childCount);

static void* g_vtable[0x260 / 4];
static int g_callOrder[2];
static int g_callCount;
static float g_scalar;
static Vector2 g_offset;
static const Vector2* g_offsetAddress;

static void __fastcall
SetOffsetScalar(void* self, void*, float scalar)
{
    FrontEndListProbe* list = (FrontEndListProbe*)self;
    g_callOrder[g_callCount++] = 0x258;
    g_scalar = scalar;

    // Model the target virtual's field write while leaving the candidate
    // responsible solely for dispatch and the exact argument.
    std::memcpy(&list->untouched[0x30], &scalar, sizeof(scalar));
}

static void __fastcall
SetOffsetVector(void* self, void*, const Vector2* offset)
{
    FrontEndListProbe* list = (FrontEndListProbe*)self;
    g_callOrder[g_callCount++] = 0x25c;
    g_offset = *offset;
    g_offsetAddress = offset;
    std::memcpy(&list->untouched[0x38], offset, sizeof(*offset));
}

static int RunCase(unsigned long childCount)
{
    FrontEndListProbe list;
    std::memset(&list, 0xa5, sizeof(list));
    list.vtable = g_vtable;
    unsigned char before[sizeof(list.untouched)];
    std::memcpy(before, list.untouched, sizeof(before));

    g_callCount = 0;
    g_scalar = -99.0f;
    g_offset.x = -99.0f;
    g_offset.y = -99.0f;
    g_offsetAddress = 0;

    CFrontEndList_InitialiseOffsets_0054c480(
        &list,
        (void*)0xfeedbeef,
        childCount);

    if (g_callCount != 2 ||
        g_callOrder[0] != 0x258 ||
        g_callOrder[1] != 0x25c)
        return 1;
    if (g_scalar != 1.0f ||
        g_offset.x != 0.0f ||
        g_offset.y != 0.0f ||
        g_offsetAddress == 0)
        return 2;

    float writtenScalar;
    Vector2 writtenOffset;
    std::memcpy(
        &writtenScalar,
        &list.untouched[0x30],
        sizeof(writtenScalar));
    std::memcpy(
        &writtenOffset,
        &list.untouched[0x38],
        sizeof(writtenOffset));
    if (writtenScalar != 1.0f ||
        writtenOffset.x != 0.0f ||
        writtenOffset.y != 0.0f)
        return 3;

    for (unsigned int i = 0; i != sizeof(list.untouched); ++i)
    {
        const bool scalarWrite = i >= 0x30 && i < 0x34;
        const bool vectorWrite = i >= 0x38 && i < 0x40;
        if (!scalarWrite && !vectorWrite &&
            list.untouched[i] != before[i])
            return 4;
    }
    return 0;
}

int main()
{
    std::memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0x258 / 4] = (void*)&SetOffsetScalar;
    g_vtable[0x25c / 4] = (void*)&SetOffsetVector;

    const unsigned long counts[] =
    {
        0,
        1,
        2,
        0x7fffffffUL,
        0xffffffffUL
    };
    for (unsigned int i = 0; i != sizeof(counts) / sizeof(counts[0]); ++i)
    {
        const int failure = RunCase(counts[i]);
        if (failure != 0)
            return (int)(10 * i + failure);
    }

    std::printf("FSE2_0054c480_TEST PASS\n");
    return 0;
}
