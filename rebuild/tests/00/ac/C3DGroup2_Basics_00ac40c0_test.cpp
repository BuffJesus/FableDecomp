// Behaviour fixture for the C3DGroup2 default constructor and AddVertexBlend (provider C3DGroup2_Basics_00ac40c0.cpp): a new
// group has bone -1 and no blends (storage at +4..+0xC zeroed); AddVertexBlend appends each blend in order through STLport
// growth; the inline count / peek accessors read them back.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include "fable_chunked_file.h"
#include "fable_3d_group2.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

int main()
{
    unsigned char storage[sizeof(C3DGroup2)];
    memset(storage, 0xCD, sizeof(storage));
    C3DGroup2* group = new (storage) C3DGroup2;
    unsigned long raw[4]; memcpy(raw, storage, 16);
    CHECK(raw[0] == 0xFFFFFFFFu && raw[1] == 0 && raw[2] == 0 && raw[3] == 0);
    CHECK(group->GetBoneIndex() == -1 && group->GetVertexBlendCount() == 0);

    for (unsigned long i = 0; i < 37; ++i)
        group->AddVertexBlend(C3DVertexBlend2(100 + i, 0.25f * (float)i));
    CHECK(group->GetVertexBlendCount() == 37);
    for (unsigned long i = 0; i < 37; ++i)
        CHECK(group->PeekVertexBlend(i).GetVertexIndex() == 100 + i && group->PeekVertexBlend(i).GetBlendValue() == 0.25f * (float)i);
    CHECK(group->GetBoneIndex() == -1);

    void* data; memcpy(&data, storage + 4, 4);
    free(data);
    if (g_fail) { printf("GROUP2_BASICS_FAIL %d\n", g_fail); return 1; }
    printf("GROUP2_BASICS_OK\n");
    return 0;
}
