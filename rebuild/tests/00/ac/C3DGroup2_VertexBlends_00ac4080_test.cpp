// Behaviour fixture for C3DGroup2's vertex-blend accessors (provider C3DGroup2_VertexBlends_00ac4080.cpp; it defines
// neither). The group is raw storage at the retail offsets fable_3d_group2.h pins, driven by qualified calls.
#include "fable_3d_group2.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char storage[sizeof(C3DGroup2)];
static unsigned char blends[3 * 8];

int main()
{
    memset(storage, 0, sizeof(storage));
    C3DGroup2* g = reinterpret_cast<C3DGroup2*>(storage);
    // Empty: both hand back the array object itself (+0x04), never a null pointer.
    CHECK((void*)g->C3DGroup2::GetVertexBlends() == storage + 4);
    CHECK((const void*)g->C3DGroup2::PeekVertexBlends() == storage + 4);
    // Three blends: start, finish, end of storage at +0x04/+0x08/+0x0C.
    unsigned char* parts[3] = { blends, blends + sizeof(blends), blends + sizeof(blends) };
    memcpy(storage + 4, parts, sizeof(parts));
    CHECK((void*)g->C3DGroup2::GetVertexBlends() == blends);
    CHECK((const void*)g->C3DGroup2::PeekVertexBlends() == blends);
    printf(g_fail ? "FAIL\n" : "GROUP2_VERTEXBLENDS_OK\n");
    return g_fail != 0;
}
