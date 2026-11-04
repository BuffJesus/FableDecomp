// Behaviour fixture for CCharString::CCharString(const CCharString&) (0x0099EC30).
// Links the compiled source object. The instance counter is the real global;
// UnassignString is a recording stand-in (unreachable here: storage_ is null
// when checked).
#define private public
#include "fable_string.h"
#undef private

#include <new>
#include <stdio.h>
#include <string.h>

// The source references the private (AAE) spelling.
#pragma comment(linker, "/alternatename:?UnassignString@CCharString@@AAEXXZ=?UnassignString@CCharString@@QAEXXZ")

fable_i32 g_CCharStringInstanceCount_013BD800;
static int g_unassigns;
void CCharString::UnassignString() { ++g_unassigns; }

int main()
{
    int fail = 0;
    CCharStringData shared;
    memset(&shared, 0, sizeof(shared));
    shared.owners = 1;

    // Copy of a string with data: shares it, one more owner, one more instance.
    unsigned long srcRaw[1] = { 0 };
    CCharString* source = reinterpret_cast<CCharString*>(srcRaw);
    source->storage_ = &shared;
    unsigned long dstRaw[3] = { 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC };
    CCharString* copy = new (&dstRaw[1]) CCharString(*source);
    if (copy->storage_ != &shared || shared.owners != 2 || g_CCharStringInstanceCount_013BD800 != 1) fail |= 1;
    if (dstRaw[0] != 0xAAAAAAAA || dstRaw[2] != 0xCCCCCCCC || source->storage_ != &shared) fail |= 2;

    // Copy of an empty string: empty, no owner change, still counted.
    unsigned long emptyRaw[1] = { 0 };
    CCharString* empty = reinterpret_cast<CCharString*>(emptyRaw);
    unsigned long dst2[1] = { 0xDDDDDDDD };
    CCharString* copy2 = new (dst2) CCharString(*empty);
    if (copy2->storage_ != 0 || g_CCharStringInstanceCount_013BD800 != 2 || shared.owners != 2) fail |= 4;

    // Self-copy (constructing over the source): comes out empty, counted.
    unsigned long selfRaw[1];
    CCharString* self = reinterpret_cast<CCharString*>(selfRaw);
    self->storage_ = &shared;
    new (selfRaw) CCharString(*self);
    if (self->storage_ != 0 || shared.owners != 2 || g_CCharStringInstanceCount_013BD800 != 3) fail |= 8;
    if (g_unassigns != 0) fail |= 16;

    if (fail == 0)
        printf("COPY_OK_MARKER\n");
    else
        printf("COPY_FAIL %d\n", fail);
    return 0;
}
