// Behaviour fixture for C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk&) (provider C3DGroup2_Construct_00ac4150.cpp).
// The group chunk's accessors live in another source, so the fixture supplies them over a fake chunk: the group must
// take the chunk's bone index and copy every vertex blend in order (STLport growth included), and an empty chunk must
// give an empty group. The group's members are read at their retail offsets (BoneIndex +0, vector +4/+8).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include "fable_chunked_file.h"
#include "fable_3d_group2.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct FakeChunk { long bone; unsigned long count; const unsigned long* blends; };
static const FakeChunk* g_chunk;
long C3DMeshFileGroupChunk::GetBoneIndex() const { return reinterpret_cast<const FakeChunk*>(this)->bone; }
unsigned long C3DMeshFileGroupChunk::GetCount() const { return reinterpret_cast<const FakeChunk*>(this)->count; }
const C3DVertexBlend2* C3DMeshFileGroupChunk::GetList() const
{
    return reinterpret_cast<const C3DVertexBlend2*>(reinterpret_cast<const FakeChunk*>(this)->blends);
}

static void Check(long bone, unsigned long count)
{
    static unsigned long blends[2 * 40];
    for (unsigned long i = 0; i < 2 * 40; ++i) blends[i] = 0x1000 * bone + i;
    FakeChunk chunk = { bone, count, blends };
    unsigned char storage[sizeof(C3DGroup2)];
    new (storage) C3DGroup2(*reinterpret_cast<const C3DMeshFileGroupChunk*>(&chunk));
    long gotBone; unsigned long* begin; unsigned long* end;
    memcpy(&gotBone, storage, 4); memcpy(&begin, storage + 4, 4); memcpy(&end, storage + 8, 4);
    CHECK(gotBone == bone);
    CHECK((unsigned long)(end - begin) == 2 * count);
    if (count) CHECK(memcmp(begin, blends, 8 * count) == 0);
    else CHECK(begin == end);
    free(begin);   // STLport's node allocator hands blocks this size to malloc
}

int main()
{
    Check(3, 0);
    Check(5, 1);
    Check(9, 40);   // several reallocations
    if (g_fail) { printf("GROUP2_CONSTRUCT_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_CONSTRUCT_OK\n");
    return 0;
}
